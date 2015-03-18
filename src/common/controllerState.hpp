#ifndef CONTROLLER_STATE_HPP
#define CONTROLLER_STATE_HPP

#include <string>
#include <iostream>

#include "action.hpp"

/**
 * The parent class of both Server and Client controller states
 *
 * @class ControllerState
 * @author Matt Hoffman && Ryan Kerr
 * @date 2015-02-10
 */
class ControllerState {
    public:

        /**
         * Public constructor
         *
         * @param state_type_desc A string representing the state we're in
         */
        ControllerState(std::string state_type_desc);

        /**
         * Destructor
         */
        virtual ~ControllerState();

        /**
         * Virtual method for handling an action
         * All ControllerStates must be able to handle an action
         *
         * @param action_to_be_handled The action that needs to be handled in the current state
         * @return A different ControllerState if the action causes a state switch. NULL otherwise
         */
        virtual ControllerState* handleAction(Action* action_to_be_handled) = 0;

        /**
         * A method that can be called whenever you get an unwanted action in handleAction
         * Prints a message to standard error
         *
         * @param action_type A string representing the action you weren't expecting
         * @return None
         */
        void reportUnhandledAction(std::string action_type);

        /**
         * A method to get the type of the current state
         * Mostly used for testing
         *
         * @return A string identifying the type of state
         */
         std::string getType( );

    private:
        //! A string containing the type of state this is
        std::string state_type;
};

#endif // CONTROLLER_STATE_HPP
