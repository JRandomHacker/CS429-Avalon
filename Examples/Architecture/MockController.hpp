
#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include <string>

#include "Action.hpp"
#include "ActionHandler.hpp"
#include "CustomActions.hpp"
#include "ControllerState.hpp"

class Controller {
public:
	Controller();
	Controller(ControllerState* starting_state);
	~Controller();

	// Thread safe adds a new action to the action_queue
	void addActionToQueue(Action* new_action);

	// Causes the controller to begin processing its action_queue.
	// This will loop infinitely and so should be in its own thread.
	void beginProcessing();

	void processAction(Action* action);

	// Inserts a new state, correctly removing any old one
	void setControllerState(ControllerState* new_state);
	// Frees the existing controller state (if any)
	void releaseControllerState();

private:
	ActionHandler action_queue;
	ControllerState* state;
};



#endif // CONTROLLER_HPP