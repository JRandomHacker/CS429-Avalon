
#ifndef CONTROLLER_STATE_HPP
#define CONTROLLER_STATE_HPP

#include <string>
#include <iostream>

#include "Action.hpp"
#include "CustomActions.hpp"

class ControllerState {
public:
	ControllerState(std::string state_type_desc);

	virtual ControllerState* handleAction(Action* action_to_be_handled) = 0;

	void reportUnhandledAction(std::string action_type);

	std::string state_type;
};

class MainScreenState : public ControllerState{
public:
	MainScreenState();

	ControllerState* handleAction(Action* action_to_be_handled);
};

class LobbyState : public ControllerState {
public:
	LobbyState();

	ControllerState* handleAction(Action* action_to_be_handled);
};

#endif // CONTROLLER_STATE_HPP