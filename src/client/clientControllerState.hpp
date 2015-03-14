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
#include "model.hpp"
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
         * @param mod A pointer to the model, to be used to update the GUI
         */
        ClientControllerState( std::string state_type_desc, Model* mod );

        /**
         * Helper function to send a protobuf to the server
         *
         * @param bufType The type of buffer that is being sent
         * @param message The protobuf to be sent, already serialized as a string (Using the protobuf method)
         * @return None
         */
        void sendProtobuf( avalon::network::buffers_t bufType, std::string message );

    protected:
        Model* model;
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
         * @param mod A pointer to the model, to be used to update the GUI
         */
        LobbyState( Model* mod );

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
         * @param mod A pointer to the model, to be used to update the GUI
         */
        VotingState( Model* mod );

        /**
         * Method to actually deal with the action we've received
         *
         * @param action_to_be_handled The action that we need to deal with in our current state
         */
        ControllerState* handleAction( Action* action_to_be_handled );
};

#endif // CLIENT_CONTROLLER_STATE_HPP
