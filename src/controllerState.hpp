
#ifndef CONTROLLER_STATE_HPP
#define CONTROLLER_STATE_HPP

#include <string>
#include <iostream>

#include "action.hpp"
#include "customActions.hpp"
#include "model.hpp"

class ControllerState {
public:
    ControllerState(std::string state_type_desc, Model* mod);
    virtual ~ControllerState();

    virtual ControllerState* handleAction(Action* action_to_be_handled) = 0;

    void reportUnhandledAction(std::string action_type);

    std::string state_type;

protected:
    Model* model;
};

class LobbyState : public ControllerState {
public:
    LobbyState( Model* mod );

    ControllerState* handleAction( Action* action_to_be_handled );
};

class VotingState : public ControllerState {
    VotingState( Model* mod );

    ControllerState* handleAction( Action* action_to_be_handled );
};

#endif // CONTROLLER_STATE_HPP
