#include "serverControllerState.hpp"
#include "serverInfo.hpp"
#include "serverCustomActions.hpp"

#include "teamselection.pb.h"
#include "vote.pb.h"
#include "voteresults.pb.h"

#ifdef _WIN32
    #include <winsock2.h>
#else
    #include <netdb.h>
#endif

// ServerControllerState {
    ServerControllerState::ServerControllerState( std::string state_type_desc, ServInfo* mod )
        : ControllerState(state_type_desc), model( mod ) {
            std::cout << "[ ServerController ] Entered " << state_type_desc << " state" << std::endl;
        }

    // Sends one player another player's information
    void ServerControllerState::sendPlayer( int playerID, int destinationID, bool allInfo ) {

        avalon::network::Player playerBuf;

        if ( allInfo ) {
            playerBuf = model->players[ playerID ]->getBuf( );
        } else {
            playerBuf = model->players[ playerID ]->getHiddenBuf( );
        }

        playerBuf.set_id( playerID );
        model->server->sendProtobuf( avalon::network::PLAYER_BUF, destinationID, playerBuf.SerializeAsString( ) );
    }

    // Sends a protobuf to all the clients
    void ServerControllerState::sendProtobufToAll( avalon::network::buffers_t bufType, std::string message ) {

        for( unsigned int i = 0; i < model->num_clients; i++ ) {
            model->server->sendProtobuf( bufType, i, message );
        }
    }
// }

// WaitingForClientsState {
    WaitingForClientsState::WaitingForClientsState( ServInfo* mod ) : ServerControllerState( "WaitingForClients", mod ) { }

    WaitingForClientsState::~WaitingForClientsState( ) { }

    ServerControllerState* WaitingForClientsState::handleAction( Action* action_to_be_handled ) {

        std::string action_type = action_to_be_handled->getMessage();

        if( action_type == "NewPlayer" ) {
            auto action = dynamic_cast< NewPlayerAction* >( action_to_be_handled );
            unsigned int playerID = action->getPlayerID( );
            std::string requestedName = action->getPlayerName( );
            if( !requestedName.empty( ) ) {
                model->players[ playerID ]->setName( requestedName );
            }

            sendStartingInfo( playerID );
        } else if( action_type == "EnterTeamSelection" ) {

            model->server->broadcastStateChange( avalon::network::ENTER_TEAM_SELECTION_BUF, model->leader );

            return new TeamSelectionState( model );
        } else {
            reportUnhandledAction( action_type );
        }

        return NULL;
    }


    // Sends the beginning information when a player connects
    void WaitingForClientsState::sendStartingInfo( int playerID ) {

        model->settingsBuf.set_client( playerID );
        model->server->sendProtobuf( avalon::network::SETTINGS_BUF, playerID, model->settingsBuf.SerializeAsString( ) );

        for( int i = 0; i < playerID; i++ ) {
            sendPlayer( i, playerID, false ); // Send each currently connected player to the new player
            sendPlayer( playerID, i, false ); // Send the new player to each player already connected
        }

        sendPlayer( playerID, playerID, true ); // Send the new player their own info
    }
// }

// TeamSelectionState {
    TeamSelectionState::TeamSelectionState( ServInfo* mod ) : ServerControllerState( "TeamSelection", mod ) { }

    TeamSelectionState::~TeamSelectionState( ) { }

    ServerControllerState* TeamSelectionState::handleAction( Action* action_to_be_handled ) {

        std::string action_type = action_to_be_handled->getMessage();

        if( action_type == "ToggleTeamMember" ) {
            auto action = dynamic_cast< ToggleTeamMemberAction* >( action_to_be_handled );
            unsigned int selector = action->getSelectorID( );
            if( selector == model->leader ) {

                unsigned int player = action->getPlayerID( );
                bool selected = toggleSelection( player );

                // Set up the protobuf to send to all the clients
                avalon::network::TeamSelection buf;
                buf.set_id( player );
                buf.set_selected( selected );
                std::string message = buf.SerializeAsString( );

                // Send the protobuf to all the clients
                sendProtobufToAll( avalon::network::TEAM_SELECTION_BUF, buf.SerializeAsString( ) );
            } else {
                std::cerr << "[ ServerController ] Received a team selection from someone who isn't the leader" << std::endl;
            }

        } else if( action_type == "ConfirmTeamSelection" ) {

            auto action = dynamic_cast< ConfirmTeamSelectionAction* >( action_to_be_handled );
            unsigned int selector = action->getSelectorID( );
            if( selector == model->leader ) {

                // TODO Add logic to make sure there aren't too many people
                model->server->broadcastStateChange( avalon::network::ENTER_TEAM_VOTE_BUF, 0 );
                return new VotingState( model );
            } else {
                std::cerr << "[ ServerController ] Received a team confirmation from someone who isn't the leader" << std::endl;
            }

        } else {
            reportUnhandledAction( action_type );
        }

        return NULL;
    }

    bool TeamSelectionState::toggleSelection( unsigned int player_id ) {
        bool wasSelected = false;

        // Look through the vector to see if the player is currently selected
        for( unsigned int i = 0; i < model->team.size( ); i++ ) {
            if( model->team[ i ] == player_id ) {
                model->team.erase( model->team.begin( ) + i );
                wasSelected = true;
            }
        }

        // If they weren't selected, select them
        if( !wasSelected ) {
            model->team.push_back( player_id );
        }

        // If they hadn't been selected, they are now
        return !wasSelected;
    }
// }

// VotingState {
    VotingState::VotingState( ServInfo* mod ) : ServerControllerState( "Voting", mod ) { }

    VotingState::~VotingState( ) { }

    ServerControllerState* VotingState::handleAction( Action* action_to_be_handled ) {

        std::string action_type = action_to_be_handled->getMessage();

        if( action_type == "TeamVote" ) {

            auto action = dynamic_cast< TeamVoteAction* >( action_to_be_handled );
            unsigned int voter = action->getVoter( );
            avalon::player_vote_t vote = action->getVote( );
            if( modifyVote( voter, vote ) ) {

                avalon::network::Vote buf;
                buf.set_id( voter );
                buf.set_vote( avalon::HIDDEN );

                sendProtobufToAll( avalon::network::VOTE_BUF, buf.SerializeAsString( ) );

                if( model->votes.size( ) == model->num_clients ) {
                    return sendVoteResults( );
                }
            }
        } else {
            reportUnhandledAction( action_type );
        }

        return NULL;
    }

    // Goes through the votes array to see if you've already voted, changes your vote ( if you did ) and returns whether anything was changed
    bool VotingState::modifyVote( unsigned int voter, avalon::player_vote_t vote ) {

        bool changed = false;
        bool exists = false;

        // Look through the vector to see if the player is currently selected
        for( unsigned int i = 0; i < model->votes.size( ); i++ ) {

            if( model->votes[ i ].first == voter ) {

                exists = true;
                avalon::player_vote_t currentVote = model->votes[ i ].second;
                if( currentVote != vote ) {

                    model->votes[ i ].second = vote;
                    changed = true;
                }
            }
        }

        // If they weren't selected, select them
        if( !exists ) {
            std::pair < unsigned int, avalon::player_vote_t > newVote( voter, vote );
            model->votes.push_back( newVote );
            changed = true;
        }

        // If they hadn't been selected, they are now
        return changed;
    }

    // Sends the vote results and the state change and changes our state.
    ServerControllerState* VotingState::sendVoteResults( ) {

        model->vote_track++;
        model->leader++;
        model->leader %= model->num_clients;
        bool passed = figureOutResultsOfVote( );

        // Sort the votes into player order
        avalon::player_vote_t sorted_votes[ model->num_clients ];
        for( std::vector< std::pair< unsigned int, avalon::player_vote_t > >::iterator it = model->votes.begin( ); it != model->votes.end( ); it++ ) {

            sorted_votes[ (*it).first ] = (*it).second;
        }

        // Create the protobuf
        avalon::network::VoteResults buf;
        buf.set_passed( passed );
        buf.set_vote_track( model->vote_track );
        for( unsigned int i = 0; i < model->num_clients; i++ ) {
            buf.add_votes( sorted_votes[ i ] );
        }

        // Send the protobuf to all the players
        for( unsigned int i = 0; i < model->num_clients; i++ ) {

            if( model->hidden_voting ) {
                buf.set_votes( i, sorted_votes[ i ] );
            }

            model->server->sendProtobuf( avalon::network::VOTE_RESULTS_BUF, i, buf.SerializeAsString( ) );

            if( model->hidden_voting ) {
                buf.set_votes( i, avalon::HIDDEN );
            }
        }

        // If we passed, go to quest vote
        model->votes.clear( );
        if( passed ) {

            model->vote_track = 0;
            model->server->broadcastStateChange( avalon::network::ENTER_QUEST_VOTE_BUF, 0 );
            // TODO change our state to quest vote
            return new TeamSelectionState( model );
        } else {

            // If these fuckers can't make up their mind, they lose.
            if( model->vote_track > model->vote_track_length ) {
                model->server->broadcastStateChange( avalon::network::ENTER_END_GAME_BUF, 0 );
                // TODO change our state to end game
                return NULL;
            }

            // Go back to the selection state, moving one step closer to annihilation
            model->server->broadcastStateChange( avalon::network::ENTER_TEAM_SELECTION_BUF, model->leader );
            return new TeamSelectionState( model );
        }
    }

    // Iterates through the votes vector, counts the votes, and returns the results
    bool VotingState::figureOutResultsOfVote( ) {

        unsigned int yes = 0;
        unsigned int no = 0;
        for( std::vector< std::pair< unsigned int, avalon::player_vote_t > >::iterator it = model->votes.begin( ); it != model->votes.end( ); it++ ) {

            if( (*it).second == avalon::YES ) {
                yes++;
            } else {
                no++;
            }
        }

        return yes > no;
    }
// }
