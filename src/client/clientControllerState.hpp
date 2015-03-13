
#ifndef CLIENT_CONTROLLER_STATE_HPP
#define CLIENT_CONTROLLER_STATE_HPP

#include <string>
#include <iostream>

#include "action.hpp"
#include "clientCustomActions.hpp"
#include "model.hpp"
#include "controllerState.hpp"

class ClientControllerState : public ControllerState {
public:
    ClientControllerState( std::string state_type_desc, Model* mod );

protected:
    Model* model;
};

class LobbyState : public ClientControllerState {
public:
    LobbyState( Model* mod );

    ControllerState* handleAction( Action* action_to_be_handled );
};

class VotingState : public ClientControllerState {
    VotingState( Model* mod );

    ControllerState* handleAction( Action* action_to_be_handled );
};

#endif // CLIENT_CONTROLLER_STATE_HPP
