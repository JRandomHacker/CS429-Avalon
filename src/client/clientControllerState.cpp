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

// Constructor for the parent class, ClientControllerState.
// Sets the description and model internally to whatever it gets passed
ClientControllerState::ClientControllerState( std::string state_type_desc, Model* mod )
    : ControllerState(state_type_desc), model( mod ) {

}

// Constructor for the VotingState, simply sets the correct state name
VotingState::VotingState( Model* mod ) : ClientControllerState( "Voting", mod ) {

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
LobbyState::LobbyState( Model* mod ) : ClientControllerState( "Lobby", mod ) {

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
        unsigned int num_clients = sBuf->players( );

        model->addData( "numberOfPlayers", num_clients );
        model->addData( "myID", sBuf->client( ) );

        for ( unsigned int i = 0; i < num_clients; i++ ) {
            model->addData( std::string( "player" ) + std::to_string( i ), NULL );
        }

        model->updateData("hasGameSettings", true);

    // We need to update one of the players in the model with the real player
    } else if( action_type == "AddPlayer" ) {

        std::cerr << "AddPlayer action handled by lobby state" << std::endl;

        auto action = dynamic_cast< AddPlayerAction* >( action_to_be_handled );
        unsigned int player_number = action->getPlayerNumber( );
        Player* p = action->getPlayerInfo( );
        std::cerr << "AddPlayer player number: " << player_number << std::endl;

        model->updateData( std::string( "player" ) + std::to_string( player_number ), p );

    // We need to send the Server our preferred name
    } else if ( action_type == "SetName" ) {

        std::cerr << "SetName action handled by lobby state" << std::endl;

        auto action = dynamic_cast< SetNameAction* >( action_to_be_handled );
        std::string new_name = action->getName( );

        // Send the server our new name

    // We don't care about the action we received, since it isn't valid in this state
    } else {

        reportUnhandledAction( action_type );
    }

    // By default, we haven't changed states
    return NULL;
}
