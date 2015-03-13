
#ifndef CONTROLLER_STATE_HPP
#define CONTROLLER_STATE_HPP

#include <string>
#include <iostream>

#include "action.hpp"

class ControllerState {
public:
    ControllerState(std::string state_type_desc);
    virtual ~ControllerState();

    virtual ControllerState* handleAction(Action* action_to_be_handled) = 0;

    void reportUnhandledAction(std::string action_type);

    std::string state_type;
};

#endif // CONTROLLER_STATE_HPP
