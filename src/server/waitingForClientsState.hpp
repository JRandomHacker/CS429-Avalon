/**
 * First state
 * Waits for clients to connect, then switches to team selection
 *
 * @class WaitingForClientsState
 * @author Ryan kerr
 * @date 2015-03-12
 */

#ifndef _WAITING_FOR_CLIENTS_STATE_HPP
#define _WAITING_FOR_CLIENTS_STATE_HPP

#include <string>
#include <iostream>

#include "action.hpp"
#include "serverInfo.hpp"
#include "serverControllerState.hpp"

namespace avalon {
namespace server {

class WaitingForClientsState : public ServerControllerState {

    public:

        /**
         * Constructor
         *
         * @param mod A pointer to the ServInfo struct that the clientController is using
         */
        WaitingForClientsState( ServInfo* mod );

        /**
         * Destructor
         */
        ~WaitingForClientsState( );

        /**
         * The workhorse that actually takes care of an action
         *
         * @param action_to_be_handled The action that this controllerState should parse
         */
        ControllerState* handleAction( Action* action_to_be_handled );

    private:
        /**
         * Helper function to send everything a player needs upon connecting to the server
         * Sends settings, their player object, and all currently connected players
         * Also sends the new player to all currently connected players
         *
         * @param playerID the ID of the player that is connecting
         * @return None
         */
        void sendStartingInfo( unsigned int playerID );

        /**
         * Helper function to decide what information the recipient should get about the player
         * and send the player info
         *
         * @param recipient The ID of the player receiving information about a new player
         * @param player The player that the recipient is receiving
         * @return None
         */
        void sendRelevantInfo( unsigned int player, unsigned int recipient );

};

} // server
} // avalon

#endif // WAITING_FOR_CLIENTS_STATE_HPP
