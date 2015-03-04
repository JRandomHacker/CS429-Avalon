#include "clientController.hpp"
#include "model.hpp"
#include <queue>
#include <semaphore.h>
#include <pthread.h>

ClientController::ClientController( Model* model, std::string host, int port ) {
	this->model = model;
	
	this->client = new Client( host, port );
	
	this->q = new std::queue< Player* >( );
	this->qSem = new sem_t;
	this->qMutex = new pthread_mutex_t;
	
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
		Player* p = q->front( );
		q->pop( );
		std::cout << "Player name: " << p->getName( ) << std::endl;
		pthread_mutex_unlock( qMutex );
	}
}
