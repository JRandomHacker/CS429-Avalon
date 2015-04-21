/**
 * Fifth state
 * Waits for everyone to send a quest vote
 *
 * @class EndGameState
 * @author Ryan Kerr && Justin Koehler
 * @date 2015-04-20
 */

#ifndef _END_GAME_STATE_HPP
#define _END_GAME_STATE_HPP

#include <string>
#include <iostream>

#include "action.hpp"
#include "globals.hpp"
#include "serverInfo.hpp"
#include "serverControllerState.hpp"

namespace avalon {
namespace server {

class EndGameState : public ServerControllerState {

    public:

        /**
         * Constructor
         *
         * @param mod A pointer to the ServInfo struct that the clientController is using
         */
        EndGameState( ServInfo* mod, avalon::alignment_t winner );

        /**
         * Destructor
         */
        ~EndGameState( );

        /**
         * The workhorse that actually takes care of an action
         *
         * @param action_to_be_handled The action that this controllerState should parse
         */
        ControllerState* handleAction( Action* action_to_be_handled );

};

} // server
} // avalon

#endif // END_GAME_STATE_HPP

