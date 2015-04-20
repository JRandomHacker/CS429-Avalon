/**
// * ClientControllerState for the end of the game
 *
 * @class FinalGameState
 * @author Ryan Kerr && Justin Koehler
 * @date 2015-04-20
 */

#ifndef _FINAL_GAME_STATE_HPP
#define _FINAL_GAME_STATE_HPP

#include <string>
#include <iostream>

#include "action.hpp"
#include "clientControllerState.hpp"
#include "globals.hpp"

namespace avalon {
namespace client {

class FinalGameState : public ClientControllerState {
    public:

        /**
         * Constructor
         *
         * @param mod A pointer to a ClientInfo struct with the clients data
         */
        FinalGameState( ClientInfo* mod );

        /**
         * Method to actually deal with the action we've received
         *
         * @param action_to_be_handled The action that we need to deal with in our current state
         */
        ControllerState* handleAction( Action* action_to_be_handled );
};

} // client
} // avalon

#endif // _FINAL_GAME_STATE_HPP_

