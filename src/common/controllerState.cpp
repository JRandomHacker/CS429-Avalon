#include "controllerState.hpp"

// Constructor. Just sets the state_type_desc to whatever we get
ControllerState::ControllerState( std::string state_type_desc ) :
    state_type( state_type_desc ) { }

// Destructor. We have nothing to clean up
ControllerState::~ControllerState( ) { }

void ControllerState::setupState( ) {

}

void ControllerState::teardownState( ) {

}

ControllerState* ControllerState::handleAction( Action* action_to_be_handled ) {
	reportUnhandledAction( action_to_be_handled->getMessage( ) );
	return NULL;
}

// Prints an error message containing the state we were in, and the action we received
void ControllerState::reportUnhandledAction( std::string action_type ) {

    std::cerr << "[ ControllerState ] Received unhandled action " << action_type << " in state " << state_type << std::endl;
}

std::string ControllerState::getType( ) {
	return state_type;
}
