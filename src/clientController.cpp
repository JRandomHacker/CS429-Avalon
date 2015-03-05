#include "clientController.hpp"
#include "model.hpp"
#include "settings.pb.h"
#include <queue>
#include <semaphore.h>
#include <pthread.h>

ClientController::ClientController( Model* model, std::string host, int port ) {
	this->model = model;
	
	this->client = new Client( host, port );
	
	this->qSem = new sem_t;
	
	this->num_clients = 0;
	this->players = NULL;
	
	sem_init( qSem, 0, 0 ); // Initialize semaphore at -1 so wait immediately blocks
	action_queue = new ActionHandler(qSem);

	client->initQueue( action_queue );

	handling_state = NULL;
	setControllerState(new LobbyState(model));
	
	pthread_t networkThread;
    if( pthread_create( &networkThread, NULL, &networkThreadHelper, this ) != 0 )
    {
        std::cerr << "Unable to start network thread" << std::endl;
        exit( EXIT_THREAD_ERROR );
    }	
}

ClientController::~ClientController( ) {
	delete client;
	delete qSem;
	delete action_queue;
	releaseControllerState();
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

void ClientController::setControllerState( ControllerState* new_state ) {
	releaseControllerState();
	handling_state = new_state;
}

void ClientController::releaseControllerState( ) {
	if (handling_state != NULL) {
		delete handling_state;
		handling_state = NULL;
	}
}

void ClientController::processAction( Action* action ) {
	ControllerState* new_state = handling_state->handleAction(action);
	if (new_state != NULL) {
		setControllerState(new_state);
	}
}

void ClientController::addActionToQueue( Action* new_action ) {
	action_queue->AddAction(new_action);
}

void ClientController::processActions( ) {
	while( true ) { 
		sem_wait( qSem );
		
		std::pair<int, std::list<Action*>::iterator> available_actions = action_queue->FreezeFrontActions();
		for (int i = 0; i < available_actions.first; i++) {
			processAction(*available_actions.second);
			available_actions.second++;
		}
		for (int i = 1; i < available_actions.first; i++) {
			sem_wait( qSem );
		}
		action_queue->ReleaseFrozenActions();
		
		/*
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
		}*/
	}
}
