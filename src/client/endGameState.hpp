/**
 * ClientControllerState for when the assassin needs to do things
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
#include "clientControllerState.hpp"
#include "globals.hpp"

namespace avalon {
namespace client {

class EndGameState : public ClientControllerState {
    public:

        /**
         * Constructor
         *
         * @param mod A pointer to a ClientInfo struct with the clients data
         */
        EndGameState( ClientInfo* mod );

        /**
         * Method to actually deal with the action we've received
         *
         * @param action_to_be_handled The action that we need to deal with in our current state
         */
        ControllerState* handleAction( Action* action_to_be_handled );
};

} // client
} // avalon

#endif // _END_GAME_STATE_HPP_

