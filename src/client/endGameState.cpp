/**
 * Implementation of the EndGameState class
 *
 * @file endGameState.cpp
 * @author Ryan Kerr && Justin Koehler
 * @date 2015-04-20
 */

#include <string>

#include "endGameState.hpp"
#include "clientCustomActionsFromGUI.hpp"
#include "clientCustomActionsFromNet.hpp"

#include "chatmessage.pb.h"

namespace avalon {
namespace client {

    // Constructor for the EndGameState, simply sets the correct state name
    EndGameState::EndGameState( ClientInfo* dat ) : ClientControllerState( "EndGame", dat ) { }

    // Figures out what needs to be done with any given action, when we're in the voting state
    ControllerState* EndGameState::handleAction( Action* action_to_be_handled ) {

        std::string action_type = action_to_be_handled->getMessage( );

        if( action_type == "EndGameInfo" ) {

            auto action = dynamic_cast< EndGameInfoAction* >( action_to_be_handled );

            data->model->updateData( "winningTeam", action->getWinner( ) );
            data->model->updateData( "endGamePlayers", action->getPlayers( ) );
            data->model->updateData( "endGameState", true );

            data->client->sendProtobuf( avalon::network::ENTER_END_GAME_BUF, "" );

        } else {
            return ClientControllerState::handleAction( action_to_be_handled );
        }

        // By default, we don't change states
        return NULL;
    }

} // client
} // avalon

