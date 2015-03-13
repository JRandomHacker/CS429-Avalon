/**
 * Main class for the server states
 * Individual states inherit from this class
 *
 * @class ServerStateHelpers
 * @author Ryan kerr
 * @date 2015-03-12
 */

#ifndef SERVER_CONTROLLER_STATE_HPP
#define SERVER_CONTROLLER_STATE_HPP

#include <string>
#include <iostream>

#include "action.hpp"
#include "serverInfo.hpp"
#include "controllerState.hpp"

class ServerControllerState : public ControllerState {

    public:
        ServerControllerState( std::string state_type_desc, ServInfo* mod );

    protected:
        ServInfo* model;

        /**
          * Helper function to send a player to another player
          *
          * @param playerID The ID of the player to be sent
          * @param destintationID The ID of the player to send the player to
          * @param allInfo Whether the player should know the role and alignment of the sent player
          * @return None
          */
        void sendPlayer( int playerID, int destinationID, bool allInfo );

        /**
          * Helper function to send a protobuf
          *
          * @param bufType The type of the buffer that is being sent
          * @param destinationID The ID of the player that the buffer should be sent to
          * @param message The protobuf to be sent, already serialized as a string (Using protobuf's method)
           * @return None
         */
        void sendProtobuf( avalon::network::buffers_t bufType, int destinationID, std::string message );
};

class WaitingForClientsState : public ServerControllerState {

    public:
        WaitingForClientsState( ServInfo* mod );
        ~WaitingForClientsState( );

        ServerControllerState* handleAction( Action* action_to_be_handled );

    private:
        /**
          * Helper function to send everything a player needs upon connecting to the server
          * Sends settings, their player object, and all currently connected players
          * Also sends the new player to all currently connected players
          *
          * @param playerID the ID of the player that is connecting
          * @return None
          */
        void sendStartingInfo( int playerID );
};

#endif // SERVER_CONTROLLER_STATE_HPP
