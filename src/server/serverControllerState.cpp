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

namespace avalon {
namespace server {

// ServerControllerState {
    ServerControllerState::ServerControllerState( std::string state_type_desc, ServInfo* mod )
        : ControllerState(state_type_desc), model( mod ) {
            std::cerr << "[ ServerController ] Entered " << state_type_desc << " state" << std::endl;
        }

    // Sends one player another player's information.
    // If allInfo is false, it hides the players affiliation
    void ServerControllerState::sendPlayer( unsigned int playerID, unsigned int destinationID, info_level_t infoLevel ) {

        avalon::network::Player playerBuf;

        switch( infoLevel ) {

            case ALLINFO:
                playerBuf = model->players[ playerID ]->getBuf( );
                break;

            case ALIGNMENTINFO:
                playerBuf = model->players[ playerID ]->getHiddenBuf( );
                playerBuf.set_alignment( model->players[ playerID ]->getAlignment( ) );
                break;

            case LOOKSLIKEMERLININFO:
                playerBuf = model->players[ playerID ]->getHiddenBuf( );
                playerBuf.set_role( avalon::MERLIN );
                break;

            case NOINFO:
            default:
                playerBuf = model->players[ playerID ]->getHiddenBuf( );
                break;
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

        // Someone is connecting
        if( action_type == "NewPlayer" ) {

            auto action = dynamic_cast< NewPlayerAction* >( action_to_be_handled );
            unsigned int playerID = action->getPlayerID( );

            // See if they requested a custom name
            std::string requestedName = action->getPlayerName( );
            if( !requestedName.empty( ) ) {
                model->players[ playerID ]->setName( requestedName );
            }

            sendStartingInfo( playerID );

        // Everyone is connected
        } else if( action_type == "EnterTeamSelection" ) {

            model->server->broadcastStateChange( avalon::network::ENTER_TEAM_SELECTION_BUF, model->leader );

            return new TeamSelectionState( model );
        } else {
            reportUnhandledAction( action_type );
        }

        // We haven't changed states
        return NULL;
    }


    // Sends the beginning information when a player connects
    void WaitingForClientsState::sendStartingInfo( unsigned int playerID ) {

        model->settingsBuf.set_client( playerID );
        model->server->sendProtobuf( avalon::network::SETTINGS_BUF, playerID, model->settingsBuf.SerializeAsString( ) );

        // Trade the information the player should know with each already connected player
        for( unsigned int i = 0; i < playerID; i++ ) {

            sendRelevantInfo( i, playerID );

            sendRelevantInfo( playerID, i );
        }

        sendPlayer( playerID, playerID, ALLINFO ); // Send the new player their own info
    }

    void WaitingForClientsState::sendRelevantInfo( unsigned int player, unsigned int recipient ) {

        bool sent = false;

        // Merlin should know of all the evil players who aren't Mordred
        if( model->players[ recipient ]->getRole( ) == avalon::MERLIN ) {
            if( model->players[ player ]->getAlignment( ) == avalon::EVIL && model->players[ player ]->getRole( ) != avalon::MORDRED ) {
                sent = true;
                sendPlayer( player, recipient, ALIGNMENTINFO );
            }
        }

        // Evil players, who aren't Oberon, should know of each other
        if( model->players[ recipient ]->getAlignment( ) == avalon::EVIL && model->players[ player ]->getAlignment( ) == avalon::EVIL ) {
            if( model->players[ recipient ]->getRole( ) != avalon::OBERON && model->players[ player ]->getRole( ) != avalon::OBERON ) {
                sent = true;
                sendPlayer( player, recipient, ALIGNMENTINFO );
            }
        }

        // Percival should know of Merlin and Morgana
        if( model->players[ recipient ]->getRole( ) == avalon::PERCIVAL ) {
            if( model->players[ player ]->getRole( ) == avalon::MERLIN || model->players[ player ]->getRole( ) == avalon::MORGANA ) {
                sent = true;
                sendPlayer( player, recipient, LOOKSLIKEMERLININFO );
            }
        }

        // No special rules, so they shouldn't know anything
        if( !sent ) {
            sendPlayer( player, recipient, NOINFO );
        }
    }
// }

// TeamSelectionState {
    TeamSelectionState::TeamSelectionState( ServInfo* mod ) : ServerControllerState( "TeamSelection", mod ) { }

    TeamSelectionState::~TeamSelectionState( ) { }

    ServerControllerState* TeamSelectionState::handleAction( Action* action_to_be_handled ) {

        std::string action_type = action_to_be_handled->getMessage();

        // We received a new team member, or are removing one
        if( action_type == "ToggleTeamMember" ) {

            auto action = dynamic_cast< ToggleTeamMemberAction* >( action_to_be_handled );
            unsigned int selector = action->getSelectorID( );

            // Make sure the player is allowed to take this action
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

        // The leader is happy with the proposed team, and wants to put it on a vote
        // Makes sure that there aren't too many selections here, rather than during proposal
        } else if( action_type == "ConfirmTeamSelection" ) {

            auto action = dynamic_cast< ConfirmTeamSelectionAction* >( action_to_be_handled );
            unsigned int selector = action->getSelectorID( );

            // Make sure the player is allowed to take this action
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

        // We haven't changed states
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

        // We received a vote from a player
        if( action_type == "TeamVote" ) {

            auto action = dynamic_cast< TeamVoteAction* >( action_to_be_handled );
            unsigned int voter = action->getVoter( );
            avalon::player_vote_t vote = action->getVote( );

            // Checks to see if the voter modified their vote
            // (It is modified if it is new, or changed)
            // If they haven't changed their vote, we don't need to do anything
            if( modifyVote( voter, vote ) ) {

                // During the voting phase, you shouldn't know other players votes
                avalon::network::Vote buf;
                buf.set_id( voter );
                buf.set_vote( avalon::HIDDEN );

                sendProtobufToAll( avalon::network::VOTE_BUF, buf.SerializeAsString( ) );

                // If we've received a vote from every player, we're ready to tally the results,
                // send the data to every player, and perform a state switch
                if( model->votes.size( ) == model->num_clients ) {

                    bool passed = sendVoteResults( );
                    return decideNewState( passed );
                }
            }
        } else {
            reportUnhandledAction( action_type );
        }

        return NULL;
    }

    // Goes through the votes array to see if you've already voted
    // changes your vote (if you did) and returns whether anything was changed
    bool VotingState::modifyVote( unsigned int voter, avalon::player_vote_t vote ) {

        bool changed = false;
        bool exists = false;

        // Look through the votes vector to see if the player voted previously
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

        // If they hadn't voted previously, just add their vote
        if( !exists ) {

            std::pair < unsigned int, avalon::player_vote_t > newVote( voter, vote );
            model->votes.push_back( newVote );
            changed = true;
        }

        return changed;
    }

    // Calculates the vote results, sends them to the players, and returns it
    bool VotingState::sendVoteResults( ) {

        // This is functionally the end of the voting phase,
        // so increase the vote track, switch the leader,
        // and tally the votes
        model->vote_track++;
        model->leader++;
        model->leader %= model->num_clients;
        bool passed = figureOutResultsOfVote( );

        // If it passed, we need to reset the vote track
        if( passed ) {
            model->vote_track = 0;
        }

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
        // We can't use broadcast, because we want to support hidden voting later
        for( unsigned int i = 0; i < model->num_clients; i++ ) {

            if( model->hidden_voting ) {

                buf.set_votes( i, sorted_votes[ i ] );
                model->server->sendProtobuf( avalon::network::VOTE_RESULTS_BUF, i, buf.SerializeAsString( ) );
                buf.set_votes( i, avalon::HIDDEN );
            } else {

                model->server->sendProtobuf( avalon::network::VOTE_RESULTS_BUF, i, buf.SerializeAsString( ) );
            }

        }
        model->votes.clear( );

        return passed;
    }

    // Figure out what state we should be entering, based off the vote
    // If it passed, go to quest vote
    // If it failed, and there is more space on vote track, go to team selection
    // If it failed, and there isn't more space on the vote track, go to end game
    ServerControllerState* VotingState::decideNewState( bool vote_passed ) {

        if( vote_passed ) {

            // TODO change our state to quest vote
            model->server->broadcastStateChange( avalon::network::ENTER_TEAM_SELECTION_BUF, model->leader );
            return new TeamSelectionState( model );

        // If these fuckers can't make up their mind, they lose.
        } else if( model->vote_track >= model->vote_track_length ) {
            // TODO change our state to end game
            model->server->broadcastStateChange( avalon::network::ENTER_END_GAME_BUF, 0 );
            return NULL;

        // Go back to the selection state, moving one step closer to annihilation
        } else {
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

} // server
} // avalon
