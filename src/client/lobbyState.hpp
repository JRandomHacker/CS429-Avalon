/**
 * ClientControllerState for when we're sitting in the lobby
 *
 * @class LobbyState
 * @author Ryan Kerr && Matt Hoffman
 * @date 2015-03-14
 */
 
#ifndef _LOBBY_STATE_HPP
#define _LOBBY_STATE_HPP

#include <string>
#include <iostream>

#include "action.hpp"
#include "clientInfo.hpp"
#include "clientControllerState.hpp"
#include "globals.hpp"

namespace avalon {
namespace client {

class LobbyState : public ClientControllerState {
    public:

        /**
         * Constructor
         *
         * @param dat A pointer to a ClientInfo struct with the clients data
         */
        LobbyState( ClientInfo* dat );

        /**
         * Method to actually deal with the action we've received
         *
         * @param action_to_be_handled The action that we need to deal with in our current state
         */
        ControllerState* handleAction( Action* action_to_be_handled );

    private:

        /*
         * Helper function to populate the roleList vector
         *
         * @param buf The GameSettings buffer to get the roles from
         * @return None
         */
        void populateSpecialRoles( avalon::network::GameSettings* buf );
};

} // client
} // avalon

#endif // _LOBBY_STATE_HPP
