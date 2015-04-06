/**
 * Parent class for for the ClientControllerStates
 * Inherits from ControllerState
 *
 * @class ClientControllerState
 * @author Ryan Kerr && Matt Hoffman
 * @date 2015-03-14
 */
#ifndef CLIENT_CONTROLLER_STATE_HPP
#define CLIENT_CONTROLLER_STATE_HPP

#include <string>
#include <iostream>

#include "action.hpp"
#include "clientInfo.hpp"
#include "controllerState.hpp"
#include "globals.hpp"

#define FROMMODEL(class, name) *data->model->referenceData< class >( name )
#define FROMMODELREF(class, name) data->model->referenceData< class >( name )

namespace avalon {
namespace client {

class ClientControllerState : public ControllerState {
    public:

        /**
         * Constructor
         *
         * @param state_type_desc A string representation of the state we're in
         * @param dat A pointer to a ClientInfo struct with the clients data
         */
        ClientControllerState( std::string state_type_desc, ClientInfo* dat );

        /**
         * Method that deals with actions in a way common to all client states.
         * 
         * @param action_to_be_handled The action that we need to deal with in our current state
         */
        ControllerState* handleAction( Action* action_to_be_handled );

    protected:
        //! The data that is being passed between the controller states
        ClientInfo* data;
};

} // client
} // avalon

#endif // CLIENT_CONTROLLER_STATE_HPP
