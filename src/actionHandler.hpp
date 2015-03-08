
#ifndef ACTION_HANDLER_HPP
#define ACTION_HANDLER_HPP

#include <list>
#include <mutex>
#include <atomic>
#include <utility>
#include <semaphore.h>
#include "Action.hpp"

class ActionHandler {
public:
	/**
	 * Public constructor
	 */
	ActionHandler( sem_t* sem );

	/**
	 * Adds a new action to the queue. Is thread safe, and can be interwoven
	 * with other writes and reads. Will block if other writes are ongoing.
	 *
	 * @param new_action The new action being added to the queue.
	 */
	void addAction( Action* new_action );

	/**
	 * Grants exclusive access to the front of the queue for reading. Blocks
	 * if other threads are reading.
	 * 
	 * @return A pair that has the number of elements that are safe to read
	 *     and an iterator to the first element to be read. Iterating past the
	 *     last safe element is undefined.
	 */
	std::pair<int, std::list<Action*>::iterator> freezeFrontActions( void );

	/**
	 * Releases read access gotten elsewhere.
	 */
	void unfreezeFrontActions( void );

	/**
	 * Removes all actions that are currently safe for reading and then
	 * releases read access.
	 */
	void releaseFrozenActions( void );

private:
	std::list<Action*> action_queue;
	std::mutex queue_append_control;
	std::mutex queue_read_control;

	sem_t* action_add_semaphore;

	int number_frozen_actions;
};

#endif // ACTION_HANDLER_HPP