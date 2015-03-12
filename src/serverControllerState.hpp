
#ifndef CLIENT_CONTROLLER_STATE_HPP
#define CLIENT_CONTROLLER_STATE_HPP

#include <string>
#include <iostream>

#include "action.hpp"
#include "serverInfo.hpp"
#include "serverCustomActions.hpp"
#include "controllerState.hpp"

class ServerControllerState : public ControllerState {
public:
    ServerControllerState( std::string state_type_desc, ServInfo* mod );

protected:
    ServInfo* model;
};

class WaitingForClientsState : public ServerControllerState {
public:
    WaitingForClientsState( ServInfo* mod );

    ServerControllerState* handleAction( Action* action_to_be_handled );
};

#endif // CLIENT_CONTROLLER_STATE_HPP
