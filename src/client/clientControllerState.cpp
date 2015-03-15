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

// Constructor for the parent class, ClientControllerState.
// Sets the description and model internally to whatever it gets passed
ClientControllerState::ClientControllerState( std::string state_type_desc, ClientInfo* dat )
    : ControllerState(state_type_desc), data( dat ) {

}

// Constructor for the VotingState, simply sets the correct state name
VotingState::VotingState( ClientInfo* dat ) : ClientControllerState( "Voting", dat ) {

}

// Figures out what needs to be done with any given action, when we're in the voting state
ControllerState* VotingState::handleAction( Action* action_to_be_handled ) {

    std::cerr << "Action being handled by main screen state" << std::endl;
    std::string action_type = action_to_be_handled->getMessage( );

    if( action_type == "EnterLobby" ) {

    } else {

        reportUnhandledAction( action_type );
    }

    // By default, we don't change states
    return NULL;
}

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
        unsigned int num_evil = sBuf->evil_count( );

        data->model->addData( "numberOfPlayers", data->num_players );
        data->model->addData( "myID", sBuf->client( ) );
        data->model->addData( "numEvilChars", num_evil );

        for ( unsigned int i = 0; i < data->num_players; i++ ) {
            data->model->addData( std::string( "player" ) + std::to_string( i ), NULL );
        }

        data->model->updateData("hasGameSettings", true);

    // We need to update one of the players in the model with the real player
    } else if( action_type == "AddPlayer" ) {

        std::cerr << "AddPlayer action handled by lobby state" << std::endl;

        auto action = dynamic_cast< AddPlayerAction* >( action_to_be_handled );
        unsigned int player_number = action->getPlayerNumber( );
        Player* p = action->getPlayerInfo( );
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

    // We don't care about the action we received, since it isn't valid in this state
    } else {

        reportUnhandledAction( action_type );
    }

    // By default, we haven't changed states
    return NULL;
}
