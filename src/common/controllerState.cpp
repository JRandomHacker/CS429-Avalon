#include "controllerState.hpp"

ControllerState::ControllerState(std::string state_type_desc) :
    state_type(state_type_desc) {
}

ControllerState::~ControllerState() {

}

void ControllerState::reportUnhandledAction(std::string action_type) {

}
