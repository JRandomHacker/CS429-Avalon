/**
 * Fifth state
 * Waits for everyone to send a quest vote
 *
 * @class AssassinState
 * @author Ryan Kerr && Justin Koehler
 * @date 2015-04-20
 */

#ifndef _ASSASSIN_STATE_HPP
#define _ASSASSIN_STATE_HPP

#include <string>
#include <iostream>

#include "action.hpp"
#include "serverInfo.hpp"
#include "serverControllerState.hpp"

namespace avalon {
namespace server {

class AssassinState : public ServerControllerState {

    public:

        /**
         * Constructor
         *
         * @param mod A pointer to the ServInfo struct that the clientController is using
         */
        AssassinState( ServInfo* mod );

        /**
         * Destructor
         */
        ~AssassinState( );

        /**
         * The workhorse that actually takes care of an action
         *
         * @param action_to_be_handled The action that this controllerState should parse
         */
        ControllerState* handleAction( Action* action_to_be_handled );

};

} // server
} // avalon

#endif // ASSASSIN_STATE_HPP

