/*
 * Implementation for all of the clientControllerStates
 *
 * @author Ryan Kerr && Matt Hoffman
 * @date 2015-03-14
 * @file clientControllerState.cpp
 */
#include "clientControllerState.hpp"
#include "clientCustomActionsFromGUI.hpp"
#include "clientCustomActionsFromNet.hpp"
#include "clientInfo.hpp"

#include "teamselection.pb.h"
#include "vote.pb.h"

#include <climits>

// ClientControllerState {
    // Constructor for the parent class, ClientControllerState.
    // Sets the description and model internally to whatever it gets passed
    ClientControllerState::ClientControllerState( std::string state_type_desc, ClientInfo* dat )
        : ControllerState(state_type_desc), data( dat ) {

            std::cerr << "[ ClientController ] Entered " << state_type_desc << " state" << std::endl;
    }
// }

// VotingState {

    // Constructor for the VotingState, simply sets the correct state name
    VotingState::VotingState( ClientInfo* dat ) : ClientControllerState( "Voting", dat ) { }

    // Figures out what needs to be done with any given action, when we're in the voting state
    ControllerState* VotingState::handleAction( Action* action_to_be_handled ) {

        std::string action_type = action_to_be_handled->getMessage( );

        if( action_type == "TeamVote" ) {

            auto action = dynamic_cast< TeamVoteAction* >( action_to_be_handled );

            avalon::network::Vote buf;
            buf.set_id( data->my_id );
            buf.set_vote( action->getPlayerVote( ) );
            data->client->sendProtobuf( avalon::network::VOTE_BUF, buf.SerializeAsString( ) );

        } else if( action_type == "VoteResults" ) {

            auto action = dynamic_cast< VoteResultsAction* >( action_to_be_handled );

            data->model->updateData( "currentVoteTrack", action->getVoteTrack( ) );

            // TODO Put everything in the model instead of printing
            if( action->getVoteResult( ) ) {
                std::cout << "[ ClientController ] Vote passed." << std::endl;
            } else {
                std::cout << "[ ClientController ] Vote failed." << std::endl;
            }

            std::cout << "[ ClientController ]";
            auto votes = action->getVotes( );
            for( unsigned int i = 0; i < votes->size( ); i++ ) {
                std::string pvote;
                switch( ( *votes )[ i ] ) {
                    case avalon::YES:
                        pvote = "pass";
                        break;
                    case avalon::NO:
                        pvote = "fail";
                        break;
                    case avalon::HIDDEN:
                        pvote = "hidden";
                        break;
                    case avalon::NO_VOTE:
                        pvote = "nothing";
                        break;
                }
                std::cout << " Player " << i << " voted " << pvote;
            }
            std::cout << std::endl;

        } else if( action_type == "ReceiveVote" ) {

            auto action = dynamic_cast< ReceiveVoteAction* >( action_to_be_handled );
            unsigned int votingPlayer = action->getVoter( );

            // TODO Update model with voting player instead of printing
            std::cout << "[ ClientController ] Received a vote from player " << votingPlayer << std::endl;

        } else if( action_type == "EnterTeamSelection" ) {

            auto action = dynamic_cast< EnterTeamSelectionAction* >( action_to_be_handled );
            data->leader = action->getLeader( );
            data->model->updateData( "leaderID", data->leader );
            data->team.clear( );
            data->model->updateData( "questingTeam", data->team );

            return new TeamSelectionState( data );
        } else {

            reportUnhandledAction( action_type );
        }

        // By default, we don't change states
        return NULL;
    }
// }

// TeamSelectionState {

    // Constructor for the TeamSelectionState, simply sets the correct state name
    TeamSelectionState::TeamSelectionState( ClientInfo* dat ) : ClientControllerState( "TeamSelection", dat ) { }

    // Figures out what needs to be done with any given action, when we're in the voting state
    ControllerState* TeamSelectionState::handleAction( Action* action_to_be_handled ) {

        std::string action_type = action_to_be_handled->getMessage( );

        if( action_type == "SelectQuestGoer" ) {
            if( data->leader == data->my_id ) {

                auto action = dynamic_cast< SelectQuestGoerAction* >( action_to_be_handled );
                avalon::network::TeamSelection buf;
                buf.set_selected( action->getSelected( ) );
                buf.set_id( action->getSelectedPlayer( ) );

                data->client->sendProtobuf( avalon::network::TEAM_SELECTION_BUF, buf.SerializeAsString( ) );
            } else {
                std::cerr << "[ ClientController ] You attempted to select a quest goer, but you're not the leader. Asshole." << std::endl;
            }

        } else if( action_type == "ModifyTeamSelection" ) {

            auto action = dynamic_cast< ModifyTeamSelectionAction* >( action_to_be_handled );

            unsigned int player = action->getSelection( );
            bool added = action->getSelected( );
            bool changed = addOrRemoveTeamMember( player, added );

            if( changed ) {
                data->model->updateData( "questingTeam", data->team );
            }

        } else if( action_type == "FinalizeTeam" ) {
            if( data->leader == data->my_id ) {

                avalon::network::TeamSelection buf;
                buf.set_selected( true );
                buf.set_id( ( int ) 1189998819991197253 );
                buf.set_finished( true );

                data->client->sendProtobuf( avalon::network::TEAM_SELECTION_BUF, buf.SerializeAsString( ) );
            } else {
                std::cerr << "[ ClientController ] You attempted to finalize team selection, but you're not the leader. Asshole." << std::endl;
            }

        } else if( action_type == "EnterVoteState" ) {

            return new VotingState( data );
        } else {

            reportUnhandledAction( action_type );
        }

        // By default, we don't change states
        return NULL;
    }

    // Adds or removes the team member, if it corresponds to a change and returns whether it changed anything
    bool TeamSelectionState::addOrRemoveTeamMember( unsigned int player, bool add ) {

        bool changed = false;
        unsigned int location;
        bool exists = false;

        // Check to see if the player is already proposed
        for( unsigned int i = 0; i < data->team.size( ); i++ ) {
            if( data->team[ i ] == player ) {
                location = i;
                exists = true;
            }
        }

        // If the players status in the team has changed, make the change
        if( add && !exists ) {
           data->team.push_back( player );
           changed = true;
        } else if( !add && exists ) {
            data->team.erase( data->team.begin( ) + location );
            changed = true;
        }

        return changed;
    }
// }

// LobbyState {
    // Constructor for the LobbyState, simply sets the correct state name
    LobbyState::LobbyState( ClientInfo* dat ) : ClientControllerState( "Lobby", dat ) { }

    // Figures out what needs to be done with any given action, when we're in the lobby state
    ControllerState* LobbyState::handleAction( Action* action_to_be_handled ) {

        std::string action_type = action_to_be_handled->getMessage( );

        // We need to add all the information about the game to the model
        if( action_type == "GameSettings" ) {

            auto action = dynamic_cast< GameSettingsAction* >( action_to_be_handled );
            avalon::network::GameSettings* sBuf = action->getSettings( );
            data->num_players = sBuf->players( );
            data->my_id = sBuf->client( );
            data->num_evil = sBuf->evil_count( );
            data->quest_track_length = sBuf->quest_track_len( );
            data->vote_track_length = sBuf->vote_track_len( );
            data->leader = UINT_MAX;

            data->model->addData( "numberOfPlayers", data->num_players );
            data->model->addData( "myID", data->my_id );
            data->model->addData( "numEvilChars", data->num_evil );
            data->model->addData( "questTrackLength", data->quest_track_length );
            data->model->addData( "voteTrackLength", data->vote_track_length );
            data->model->addData( "leaderID", data->leader );
            data->model->addData( "questingTeam", data->team );
            data->model->addData( "currentVoteTrack", (unsigned int) 0 );

            for ( unsigned int i = 0; i < data->num_players; i++ ) {
                data->model->addData( std::string( "player" ) + std::to_string( i ), NULL );
            }

            delete sBuf;
            data->model->updateData("hasGameSettings", true);

        // We need to update one of the players in the model with the real player
        } else if( action_type == "AddPlayer" ) {

            auto action = dynamic_cast< AddPlayerAction* >( action_to_be_handled );
            unsigned int player_number = action->getPlayerNumber( );
            Player* p = action->getPlayerInfo( );
            data->players.push_back( p );

            data->model->updateData( std::string( "player" ) + std::to_string( player_number ), p );

        // We need to send the Server our preferred name
        } else if ( action_type == "SetName" ) {

            auto action = dynamic_cast< SetNameAction* >( action_to_be_handled );
            Player p( action->getName( ), avalon::UNKNOWN_ROLE, avalon::UNKNOWN_ALIGN );
            avalon::network::Player buf = p.getBuf( );
            buf.set_id( data->my_id );
            data->client->sendProtobuf( avalon::network::PLAYER_BUF, buf.SerializeAsString( ) );

        } else if ( action_type == "EnterTeamSelection" ) {

            auto action = dynamic_cast< EnterTeamSelectionAction* >( action_to_be_handled );
            data->leader = action->getLeader( );
            data->model->updateData( "leaderID", data->leader );
            data->team.clear( );
            data->model->updateData( "questingTeam", data->team );

            return new TeamSelectionState( data );
        // We don't care about the action we received, since it isn't valid in this state
        } else {

            reportUnhandledAction( action_type );
        }

        // By default, we haven't changed states
        return NULL;
    }
// }
