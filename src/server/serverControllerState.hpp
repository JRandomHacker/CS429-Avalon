/*
 * File containing all the server controller states
 */
#ifndef SERVER_CONTROLLER_STATE_HPP
#define SERVER_CONTROLLER_STATE_HPP

#include <string>
#include <iostream>

#include "action.hpp"
#include "serverInfo.hpp"
#include "controllerState.hpp"

/**
 * Main class for the server states
 * Individual states inherit from this class
 *
 * @class ServerControllerState
 * @author Ryan kerr
 * @date 2015-03-12
 */
class ServerControllerState : public ControllerState {

    public:

        /**
         * Constructor
         *
         * @param state_type_desc A string representation of the state we're in
         * @param mod A pointer to the ServInfo struct that the clientController is using
         */
        ServerControllerState( std::string state_type_desc, ServInfo* mod );

    protected:
        //! A pointer to the model containing the game state
        ServInfo* model;

        /**
         * Helper function to send a player to another player
         *
         * @param playerID The ID of the player to be sent
         * @param destinationID The ID of the player to send the player to
         * @param allInfo Whether the player should know the role and alignment of the sent player
         * @return None
         */
        void sendPlayer( int playerID, int destinationID, bool allInfo );

        /**
         * Helper function to send a protobuf to all clients
         *
         * @param bufType The type of buffer that is being sent
         * @param message The serialized string version of the protobuf
         * @return None
         */
        void sendProtobufToAll( avalon::network::buffers_t bufType, std::string message );

};

/**
 * First state
 * Waits for clients to connect, then switches to team selection
 *
 * @class WaitingForClientsState
 * @author Ryan kerr
 * @date 2015-03-12
 */
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

/**
 * Second state
 * Waits for the leader to send us team members they want on a mission
 *
 * @class TeamSelectionState
 * @author Ryan Kerr
 * @date 2015-03-12
 */
class TeamSelectionState : public ServerControllerState {

    public:

        /**
         * Constructor
         *
         * @param mod A pointer to the ServInfo struct that the clientController is using
         */
        TeamSelectionState( ServInfo* mod );

        /**
         * Destructor
         */
        ~TeamSelectionState( );

        /**
         * The workhorse that actually takes care of an action
         *
         * @param action_to_be_handled The action that this controllerState should parse
         */
        ServerControllerState* handleAction( Action* action_to_be_handled );

    private:
        /*
         * A helper to toggle the selection of a player
         * Removes the player from the team selection vector if they're in it
         * Adds them otherwise
         *
         * @param player_id The id of the player to toggle
         * @return Whether the player is now selected or not
         */
        bool toggleSelection( unsigned int player_id );
};

#endif // SERVER_CONTROLLER_STATE_HPP
