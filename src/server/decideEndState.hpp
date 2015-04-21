/**
 * Factory to decide the proper end game state and return it
 *
 * @file decideEndState.hpp
 * @author Ryan Kerr && Justin Koehler
 * @date 2015-04-20
 */

#ifndef _DECIDE_END_STATE_HPP
#define _DECIDE_END_STATE_HPP

#include "serverControllerState.hpp"
#include "serverInfo.hpp"
#include "globals.hpp"
#include "assassinState.hpp"
#include "endGameState.hpp"

namespace avalon {
namespace server {

    /**
     * Function that returns the proper end game state
     *
     * @param model The model the game state is using
     * @return EndGameState or FinalState depending on assassin info
     */
    ControllerState* decideEndState( ServInfo* model );

} // server
} // avalon

#endif // _DECIDE_END_STATE_HPP
