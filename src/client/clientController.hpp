/**
 * A class for the game controller on the client side
 *
 * @class ClientController
 * @author Ryan Kerr && Justin Koehler && Matt Hoffman
 * @date 2015-03-03
 */

#ifndef _CLIENTCONTROLLER_HPP
#define _CLIENTCONTROLLER_HPP

#include "player.hpp"
#include "model.hpp"
#include "client.hpp"
#include "globals.hpp"
#include "clientControllerState.hpp"
#include "actionHandler.hpp"
#include "action.hpp"
#include <semaphore.h>
#include <string>

class ClientController {
    private:

        // The model to post changes to
        Model* model;

        // A client for networking to the server
        Client* client;
        ClientInfo* data;

        // The queue to put actions into so that the separate thread can process them later
        // (And associated helpers)
        ActionHandler* action_queue;
        ControllerState* handling_state = NULL;
        sem_t* qSem;

        /*
         * Loops while the game is running, telling the Client to look for new data from the server
         *
         * @return None
         */
        void networkThreadFunc( );

        /*
         * Helper function to be called by pthread_create
         * This is a workaround for the fact that you can't pass a class function to pthread_create
         *
         * @param obj Expects the current object (this)
         * @return None
         */
        static void* networkThreadHelper( void* obj );

        /*
         * Helper function to actually process an action
         * Called by processActions( ) whenever it pulls something from the queue
         *
         * @param action A pointer to the action that should be processed
         * @return None
         */
        void processAction( Action* action );

        /*
         * Helper function to change the ControllerState after processing an action
         * If the action causes us to change state, this function can be called to
         * clear up the current states allocations, and switch to the new state
         *
         * @param new_state The new state to put the ControllerState in
         * @return None
         */
        void setControllerState( ControllerState* new_state );

        /*
         * Helper function to clear the ControllerState's data
         *
         * @return None
         */
        void releaseControllerState( );

    public:

        /**
         * Contructor
         *
         * @param model A pointer to the model that the GUI is watching
         */
        ClientController( Model* model );

        /**
         * Destructor
         * Will not deallocate model, that is the GUI's job
         */
        ~ClientController( );

        /**
         * Initializes the network connection and thread
         *
         * @param host The hostname or ip address of the server to connect to
         * @param port The port number of the server to connect to
         * @return EXIT_SUCCESS on success, an error code otherwise
         */
        int spawnNetwork( std::string host, int port );

        /**
         * This method should be called on its own thread, once the controller has been set up.
         * This method simply infinite loops, waiting for actions, and processing them as they appear
         * in the queue
         *
         * @return None
         */
        void processActions( );


        /**
         * Add an action to the controller's Action Queue
         * Actions will be processed in the order they are sent to the controller
         *
         * @param new_action The action to be appended to the process queue
         * @return None
         */
        void addActionToQueue( Action* new_action );
};


#endif
