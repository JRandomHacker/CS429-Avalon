#ifndef _CLIENTCONTROLLER_HPP
#define _CLIENTCONTROLLER_HPP

#include "player.hpp"
#include "model.hpp"
#include "client.hpp"
#include <queue>
#include <semaphore.h>
#include <pthread.h>

class ClientController {
	private:
		Model* model;
		Client* client;
		std::queue< Player* >* q; // Replace with action queue stuff
		sem_t* qSem;
		pthread_mutex_t* qMutex;
		
		void networkThreadFunc( );
		
		static void* networkThreadHelper( void* obj );
		
	public:
		ClientController( Model* model, std::string host, int port );
		~ClientController( );
		
		void processActions( );
};

#endif
