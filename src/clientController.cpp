#include "clientController.hpp"
#include "model.hpp"
#include "settings.pb.h"
#include <queue>
#include <semaphore.h>
#include <pthread.h>

ClientController::ClientController( Model* model, std::string host, int port ) {
	this->model = model;
	
	this->client = new Client( host, port );
	
	this->q = new std::queue< tempAction >( );
	this->qSem = new sem_t;
	this->qMutex = new pthread_mutex_t;
	
	this->num_clients = 0;
	this->players = NULL;
	
	sem_init( qSem, 0, 0 ); // Initialize semaphore at -1 so wait immediately blocks
	pthread_mutex_init( qMutex, NULL );
	
	client->initQueue( q, qSem, qMutex );
	
	pthread_t networkThread;
    if( pthread_create( &networkThread, NULL, &networkThreadHelper, this ) != 0 )
    {
        std::cerr << "Unable to start network thread" << std::endl;
        exit( EXIT_THREAD_ERROR );
    }	
}

ClientController::~ClientController( ) {
	delete client;
	delete q;
	delete qSem;
	delete qMutex;
}

void* ClientController::networkThreadHelper( void* obj ) {
	static_cast< ClientController* >( obj )->networkThreadFunc( );
	return NULL;
}

void ClientController::networkThreadFunc( ) {
	while( true ) {
		client->waitForData( );
	}
}

void ClientController::processActions( ) {
	while( true ) { 
		sem_wait( qSem );
		pthread_mutex_lock( qMutex );
		tempAction t = q->front( );
		q->pop( );
		pthread_mutex_unlock( qMutex );
		
		switch( t.flag ) {
			case 1:
				// Do player stuff
				if ( players == NULL ) {
					std::cerr << "Recvd a player before settings." << std::endl;
					exit( 172342 );
				} else {
					players[ t.playerID ] = (Player*)t.data;
					std::cout << "Received player " << t.playerID;
					if ( t.playerID == myID ) {
						std::cout << " (me)";
					}
					std::cout << std::endl;
				}
				break;
			case 2:
			{
				// Do settings stuff
				avalon::network::GameSettings* sBuf = (avalon::network::GameSettings*)t.data;
				num_clients = sBuf->players( );
				std::cout << "Waiting for " << num_clients << " players." << std::endl;
				players = new Player*[ num_clients ];
				myID = sBuf->client( );
				break;
			}
			default:
				break;
		}
	}
}
