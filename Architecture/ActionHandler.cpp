#include "ActionHandler.hpp"

// Public constructor
ActionHandler::ActionHandler() {

}

// Adds an action to the queue
void ActionHandler::AddAction(Action new_action) {
	queue_append_control.lock();

	action_queue.push_back(new_action);

	queue_append_control.unlock();
}

// Gets access to the front of the queue for reading with a number of safe
// elements
std::pair<int, std::list<Action>::iterator> ActionHandler::FreezeFrontActions() {
	queue_read_control.lock();

	queue_append_control.lock();
	number_frozen_actions = action_queue.size();
	queue_append_control.unlock();

	if (number_frozen_actions == 0) {
		queue_read_control.unlock();
	}

	return std::make_pair(number_frozen_actions, action_queue.begin());
}

// Deletes the currently safe to read actions and releases read access
void ActionHandler::ReleaseFrozenActions() {
	for (int i = 0; i < number_frozen_actions; i++) {
		action_queue.pop_front();
	}
	UnfreezeFrontActions();
}

// Releases read access to the front of the queue
void ActionHandler::UnfreezeFrontActions() {
	number_frozen_actions = 0;
	queue_read_control.unlock();
}