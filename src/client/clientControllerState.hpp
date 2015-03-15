/*
 * File with class definitions for all the clientControllerStates
 * May be refactored into separate files later?
 *
 */
#ifndef CLIENT_CONTROLLER_STATE_HPP
#define CLIENT_CONTROLLER_STATE_HPP

#include <string>
#include <iostream>

#include "action.hpp"
#include "clientInfo.hpp"
#include "controllerState.hpp"
#include "globals.hpp"

/**
 * Parent class for for the ClientControllerStates
 * Inherits from ControllerState
 *
 * @class ClientControllerState
 * @author Ryan Kerr && Matt Hoffman
 * @date 2015-03-14
 */
class ClientControllerState : public ControllerState {
    public:

        /**
         * Constructor
         *
         * @param state_type_desc A string representation of the state we're in
         * @param dat A pointer to a ClientInfo struct with the clients data
         */
        ClientControllerState( std::string state_type_desc, ClientInfo* dat );

    protected:
        //! The data that is being passed between the controller states
        ClientInfo* data;
};

/**
 * ClientControllerState for when we're sitting in the lobby
 *
 * @class LobbyState
 * @author Ryan Kerr && Matt Hoffman
 * @date 2015-03-14
 */
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
};

/**
 * ClientControllerState for when we're voting on something
 *
 * @class VotingState
 * @author Ryan Kerr && Matt Hoffman
 * @date 2015-03-14
 */
class VotingState : public ClientControllerState {
    public:

        /**
         * Constructor
         *
         * @param mod A pointer to a ClientInfo struct with the clients data
         */
        VotingState( ClientInfo* mod );

        /**
         * Method to actually deal with the action we've received
         *
         * @param action_to_be_handled The action that we need to deal with in our current state
         */
        ControllerState* handleAction( Action* action_to_be_handled );
};

#endif // CLIENT_CONTROLLER_STATE_HPP
