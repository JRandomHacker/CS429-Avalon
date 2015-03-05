#ifndef _CLIENTCONTROLLER_HPP
#define _CLIENTCONTROLLER_HPP

#include "player.hpp"
#include "model.hpp"
#include "client.hpp"
#include "globals.hpp"
#include "ControllerState.hpp"
#include "ActionHandler.hpp"
#include "Action.hpp"
#include <queue>
#include <semaphore.h>
#include <pthread.h>

class ClientController {
	private:
		Model* model;
		Client* client;

		ActionHandler* action_queue;
		ControllerState* handling_state;

		sem_t* qSem;
		
		unsigned int num_clients;
		unsigned int myID;
		Player** players;
		
		void networkThreadFunc( );
		
		static void* networkThreadHelper( void* obj );

		void processAction( Action* action );

		void setControllerState( ControllerState* new_state );
		void releaseControllerState( );
		
	public:
		ClientController( Model* model, std::string host, int port );
		~ClientController( );
		
		// Begins an infinite loop that keeps processing actions as long as
		// they exist
		void processActions( );
		void addActionToQueue( Action* new_action );
};


#endif
