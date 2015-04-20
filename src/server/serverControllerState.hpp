/**
 * Main class for the server states
 * Individual states inherit from this class
 *
 * @class ServerControllerState
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

namespace avalon {
namespace server {


class ServerControllerState : public ControllerState {

    public:

        /**
         * Constructor
         *
         * @param state_type_desc A string representation of the state we're in
         * @param mod A pointer to the ServInfo struct that the clientController is using
         */
        ServerControllerState( std::string state_type_desc, ServInfo* mod );

        /**
         * Method that deals with actions in a way common to all server states.
         * 
         * @param action_to_be_handled The action that we need to deal with in our current state
         */
        ControllerState* handleAction( Action* action_to_be_handled );

        /**
         * Helper function to see if the evil players have currently won
         *
         * @return Whether the evil players have won
         */
        static bool badGuysWon( ServInfo* model );

        /**
         * Helper function to see if the good players have currently won
         *
         * @return Whether the good players have won (sans assassin)
         */
        static bool goodGuysWon( ServInfo* model );

    protected:
        //! A pointer to the model containing the game state
        ServInfo* model;

        //! The enum that sendPlayer expects
        enum info_level_t {
            //! Send the players role and alignment
            ALLINFO,
            //! Send the players alignment
            ALIGNMENTINFO,
            //! Set the players role to Merlin (Used for Percival)
            LOOKSLIKEMERLININFO,
            //! Send neither the players role, nor their alignment
            NOINFO
        };

        /**
         * Helper function to send a player to another player
         *
         * @param playerID The ID of the player to be sent
         * @param destinationID The ID of the player to send the player to
         * @param infoLevel The amount of information you wish to be sent
         * @return None
         */
        void sendPlayer( unsigned int playerID, unsigned int destinationID, info_level_t infoLevel );

        /**
         * Helper function to send a protobuf to all clients
         *
         * @param bufType The type of buffer that is being sent
         * @param message The serialized string version of the protobuf
         * @return None
         */
        void sendProtobufToAll( avalon::network::buffers_t bufType, std::string message );

        /**
         * Helper function to see if the evil players have currently won
         *
         * @return Whether the evil players have won
         */
        bool badGuysWon( );

        /**
         * Helper function to see if the good players have currently won
         *
         * @return Whether the good players have won (sans assassin)
         */
        bool goodGuysWon( );

};

} // server
} // avalon

#endif // SERVER_CONTROLLER_STATE_HPP
