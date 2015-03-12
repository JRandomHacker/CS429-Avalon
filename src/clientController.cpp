/*
 * Implementation of ClientController class
 *
 * @file clientController.cpp
 * @author Ryan Kerr && Justin Koehler && Matt Hoffman
 * @date 2015-03-03
 */
#include "clientController.hpp"
#include "model.hpp"
#include <semaphore.h>
#include <pthread.h>

// Constructor
ClientController::ClientController( Model* model ) {

    this->model = model;
    model->addData("hasGameSettings", false);
    this->client = new Client( );

    this->qSem = new sem_t;

    sem_init( qSem, 0, 0 ); // Initialize semaphore to 0 so wait immediately blocks

    // Set up the queue for actions so the network can send us actions
    action_queue = new ActionHandler( qSem );
    client->initQueue( action_queue );

    handling_state = NULL;
    setControllerState( new LobbyState( model ) ); // When the controller first starts, we're in the lobby waiting for players

}

// Destructor
ClientController::~ClientController( ) {

    // We didn't allocate the model, so leave it for the GUI
    delete client;
    delete qSem;
    delete action_queue;
    releaseControllerState( );
}

int ClientController::spawnNetwork( std::string host, int port ) {

    int retval = client->initClient( host, port );
    if( retval != EXIT_SUCCESS ) {
        return retval;
    }

    // Start the Client listening for server data
    pthread_t networkThread;
    if( pthread_create( &networkThread, NULL, &networkThreadHelper, this ) != 0 )
    {
        std::cerr << "Unable to start network thread" << std::endl;
        return EXIT_THREAD_ERROR;
    }

    return EXIT_SUCCESS;
}


// Helper to spawn the client networking
void* ClientController::networkThreadHelper( void* obj ) {

    // Cast the obj to a pointer to ourself so we can start the method we actually wanted
    static_cast< ClientController* >( obj )->networkThreadFunc( );
    return NULL;
}

// Actual network thread function
void ClientController::networkThreadFunc( ) {
    while( true ) {
        client->waitForData( );
    }
}

// Helper function to change the controller's state
void ClientController::setControllerState( ControllerState* new_state ) {
    releaseControllerState( ); // Clean up the old ControllerState before changing
    handling_state = new_state;
}

// Helper function to clean up the ControllerState
void ClientController::releaseControllerState( ) {

    // Make sure we'd actually GOTTEN a state
    if( handling_state != NULL ) {
        delete handling_state;
        handling_state = NULL;
    }
}

// Helper function to process an action
// Calls the ActionHandler to do the work and changes our state accordingly
void ClientController::processAction( Action* action ) {
    ControllerState* new_state = handling_state->handleAction( action );
    if( new_state != NULL ) {
        setControllerState( new_state );
    }
}

void ClientController::addActionToQueue( Action* new_action ) {
    action_queue->addAction(new_action);
}

// Infinite loop, waiting for actions from either the GUI or network
// and processing them as they come in
void ClientController::processActions( ) {
    while( true ) {
        sem_wait( qSem ); // Wait until there is an action to process, so we don't spinlock

        // Grab all the currently available actions
        std::pair< int, std::list< Action* >::iterator > available_actions = action_queue->freezeFrontActions( );

        // Iterate through the available actions and process them
        for( int i = 0; i < available_actions.first; i++ ) {
            processAction( *available_actions.second );
            available_actions.second++;
        }

        // Decrement the semaphore for as many actions as we got
        // This is a bit hacky, and should probably be changed later
        for( int i = 1; i < available_actions.first; i++ ) {
            sem_wait( qSem );
        }

        action_queue->releaseFrozenActions( );
    }
}
