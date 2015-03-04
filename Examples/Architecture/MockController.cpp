#include "MockController.hpp"

Controller::Controller() : state(NULL) {
	setControllerState(new MainScreenState());
}

Controller::Controller(ControllerState* starting_state) : state(NULL) {
	setControllerState(starting_state);
}

Controller::~Controller() {
	releaseControllerState();
}

void Controller::releaseControllerState() {
	if (state != NULL) {
		delete state;
	}
}

void Controller::setControllerState(ControllerState* new_state) {
	releaseControllerState();
	state = new_state;
}

void Controller::addActionToQueue(Action* new_action) {
	action_queue.AddAction(new_action);
}

void Controller::beginProcessing() {
	while (true) {
		std::pair<int, std::list<Action*>::iterator> available_actions = action_queue.FreezeFrontActions();
		for (int i = 0; i < available_actions.first; i++) {
			processAction(*available_actions.second);
			available_actions.second++;
		}
		action_queue.ReleaseFrozenActions();
	}
}

void Controller::processAction(Action* action) {
	ControllerState* new_state = state->handleAction(action);
	if (new_state != NULL) {
		setControllerState(new_state);
	}
}
