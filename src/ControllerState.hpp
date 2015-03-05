
#ifndef CONTROLLER_STATE_HPP
#define CONTROLLER_STATE_HPP

#include <string>
#include <iostream>

#include "Action.hpp"
#include "CustomActions.hpp"
#include "model.hpp"

class ControllerState {
public:
	ControllerState(std::string state_type_desc, Model* mod);
	virtual ~ControllerState();

	virtual ControllerState* handleAction(Action* action_to_be_handled) = 0;

	void reportUnhandledAction(std::string action_type);

	std::string state_type;

private:
	Model* model;
};

class LobbyState : public ControllerState {
public:
	LobbyState( );

	ControllerState* handleAction( Action* action_to_be_handled );
};

class VotingState : public ControllerState {
	VotingState( );

	ControllerState* handleAction( Action* action_to_be_handled );
};

#endif // CONTROLLER_STATE_HPP