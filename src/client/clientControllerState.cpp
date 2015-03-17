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

// ClientControllerState {
    // Constructor for the parent class, ClientControllerState.
    // Sets the description and model internally to whatever it gets passed
    ClientControllerState::ClientControllerState( std::string state_type_desc, ClientInfo* dat )
        : ControllerState(state_type_desc), data( dat ) {

    }
// }

// VotingState {

    // Constructor for the VotingState, simply sets the correct state name
    VotingState::VotingState( ClientInfo* dat ) : ClientControllerState( "Voting", dat ) {

    }

    // Figures out what needs to be done with any given action, when we're in the voting state
    ControllerState* VotingState::handleAction( Action* action_to_be_handled ) {

        std::string action_type = action_to_be_handled->getMessage( );

        if( action_type == "TeamVote" ) {

            auto action = dynamic_cast< TeamVoteAction* >( action_to_be_handled );

            avalon::network::Vote buf;
            buf.set_id( data->my_id );
            buf.set_vote( action->getPlayerVote( ) );
            data->client->sendProtobuf( avalon::network::VOTE_BUF, buf.SerializeAsString( ) );
        } else {

            reportUnhandledAction( action_type );
        }

        // By default, we don't change states
        return NULL;
    }
// }

// TeamSelectionState {

    // Constructor for the TeamSelectionState, simply sets the correct state name
    TeamSelectionState::TeamSelectionState( ClientInfo* dat ) : ClientControllerState( "TeamSelection", dat ) {

    }

    // Figures out what needs to be done with any given action, when we're in the voting state
    ControllerState* TeamSelectionState::handleAction( Action* action_to_be_handled ) {

        std::string action_type = action_to_be_handled->getMessage( );

        if( action_type == "SelectQuestGoerAction" ) {
            if( data->leader == data->my_id ) {

                auto action = dynamic_cast< SelectQuestGoerAction* >( action_to_be_handled );
                avalon::network::TeamSelection buf;
                buf.set_selected( action->getSelected( ) );
                buf.set_id( action->getSelectedPlayer( ) );

                data->client->sendProtobuf( avalon::network::TEAM_SELECTION_BUF, buf.SerializeAsString( ) );
            } else {
                std::cerr << "[ ControllerState ] You attempted to select a quest goer, but you're not the leader. Asshole." << std::endl;
            }

        } else if( action_type == "FinalizeTeamAction" ) {
            if( data->leader == data->my_id ) {

                avalon::network::TeamSelection buf;
                buf.set_selected( true );
                buf.set_id( ( int ) 1189998819991197253 );
                buf.set_finished( true );

                data->client->sendProtobuf( avalon::network::TEAM_SELECTION_BUF, buf.SerializeAsString( ) );
            } else {
                std::cerr << "[ ControllerState ] You attempted to finalize team selection, but you're not the leader. Asshole." << std::endl;
            }

        } else if( action_type == "EnterVotingState" ) {

            return new VotingState( data );
        } else {

            reportUnhandledAction( action_type );
        }

        // By default, we don't change states
        return NULL;
    }
// }

// LobbyState {
    // Constructor for the LobbyState, simply sets the correct state name
    LobbyState::LobbyState( ClientInfo* dat ) : ClientControllerState( "Lobby", dat ) {

    }

    // Figures out what needs to be done with any given action, when we're in the lobby state
    ControllerState* LobbyState::handleAction( Action* action_to_be_handled ) {

        std::cerr << "Action being handled by lobby state" << std::endl;
        std::string action_type = action_to_be_handled->getMessage( );

        // We need to add all the information about the game to the model
        if( action_type == "GameSettings" ) {

            std::cerr << "GameSettings action handled by lobby state" << std::endl;

            auto action = dynamic_cast< GameSettingsAction* >( action_to_be_handled );
            avalon::network::GameSettings* sBuf = action->getSettings( );
            data->num_players = sBuf->players( );
            data->my_id = sBuf->client( );
            data->num_evil = sBuf->evil_count( );

            data->model->addData( "numberOfPlayers", data->num_players );
            data->model->addData( "myID", data->my_id );
            data->model->addData( "numEvilChars", data->num_evil );

            for ( unsigned int i = 0; i < data->num_players; i++ ) {
                data->model->addData( std::string( "player" ) + std::to_string( i ), NULL );
            }

            delete sBuf;
            data->model->updateData("hasGameSettings", true);

        // We need to update one of the players in the model with the real player
        } else if( action_type == "AddPlayer" ) {

            std::cerr << "AddPlayer action handled by lobby state" << std::endl;

            auto action = dynamic_cast< AddPlayerAction* >( action_to_be_handled );
            unsigned int player_number = action->getPlayerNumber( );
            Player* p = action->getPlayerInfo( );
            data->players.push_back( p );
            std::cerr << "AddPlayer player number: " << player_number << std::endl;

            data->model->updateData( std::string( "player" ) + std::to_string( player_number ), p );

        // We need to send the Server our preferred name
        } else if ( action_type == "SetName" ) {

            std::cerr << "SetName action handled by lobby state" << std::endl;

            auto action = dynamic_cast< SetNameAction* >( action_to_be_handled );
            Player p( action->getName( ), avalon::UNKNOWN_ROLE, avalon::UNKNOWN_ALIGN );
            avalon::network::Player buf = p.getBuf( );
            buf.set_id( data->my_id );
            data->client->sendProtobuf( avalon::network::PLAYER_BUF, buf.SerializeAsString( ) );

        } else if ( action_type == "EnterTeamSelection" ) {

            std::cerr << "Entering team selection state" << std::endl;

            auto action = dynamic_cast< EnterTeamSelectionAction* >( action_to_be_handled );
            data->leader = action->getLeader( );

            return new TeamSelectionState( data );
        // We don't care about the action we received, since it isn't valid in this state
        } else {

            reportUnhandledAction( action_type );
        }

        // By default, we haven't changed states
        return NULL;
    }
// }
