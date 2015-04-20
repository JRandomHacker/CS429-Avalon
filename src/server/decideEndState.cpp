/**
 * Implementation of the function to decide the proper end game state
 *
 * @file decideEndState.cpp
 * @author Ryan Kerr && Justin Koehler
 * @date 2015-04-20
 */

#include "decideEndState.hpp"
#include "serverControllerState.hpp"

namespace avalon {
namespace server {

ControllerState* decideEndState( ServInfo* model ) {

    if( ServerControllerState::badGuysWon( model ) ) {

        model->server->broadcastStateChange( avalon::network::ENTER_FINAL_GAME_BUF, 0 );
        return new FinalGameState( model, avalon::EVIL );
    } else if( !model->settingsBuf.assassin( ) ) {

        model->server->broadcastStateChange( avalon::network::ENTER_FINAL_GAME_BUF, 0 );
        return new FinalGameState( model, avalon::GOOD );
    } else {

        model->server->broadcastStateChange( avalon::network::ENTER_END_GAME_BUF, 0 );
        return new EndGameState( model );
    }
}

} // server
} // avalon
