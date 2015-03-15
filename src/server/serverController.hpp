/**
 * A class for the game controller on the server side
 *
 * @class ServerController
 * @author Ryan Kerr
 * @date 2015-03-11
 */

#ifndef _SERVERCONTROLLER_HPP
#define _SERVERCONTROLLER_HPP

#include "player.hpp"
#include "globals.hpp"
#include "server.hpp"
#include "serverControllerState.hpp"
#include "actionHandler.hpp"
#include "action.hpp"
#include <semaphore.h>

class ServerController {
    private:

        // Holds the current game state
        ServInfo* model;
        bool server_started;

        // The thread that the Server is running on
        pthread_t network_thread;

        // The queue to put actions into so that the separate thread can process them later
        // (And associated helpers)
        ActionHandler* action_queue;
        ServerControllerState* handling_state;
        sem_t* qSem;

        /*
         * Loops while the game is running, telling the Server to look for new data from the client
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
         * Helper function to change the ServerState after processing an action
         * If the action causes us to change state, this function can be called to
         * clear up the current states allocations, and switch to the new state
         *
         * @param new_state The new state to put the ControllerState in
         * @return None
         */
        void setServerState( ServerControllerState* new_state );

        /*
         * Helper function to clear the ServerState's data
         *
         * @return None
         */
        void releaseServerState( );

        /*
         * Helper function for the contructor to actually initialize the players array
         *
         * @param special_roles A vector of the special characters we want to use
         * @return None
         */
        void initModelCharacters( std::vector< avalon::special_roles_t > special_roles );

        /*
         * Helper function for the contructor to actually initialize the players array
         *
         * @param special_roles A vector of the special characters we want to use
         * @return EXIT_SUCCESS on success, an error code otherwise
         */
        int initModelPlayer( std::vector< avalon::special_roles_t > special_roles );

    public:

        /**
         * Contructor
         *
         * @param model A pointer to the model that the GUI is watching
         * @param host The hostname or ip address of the server to connect to
         * @param port The port number of the server to connect to
         *
         */
        ServerController( ServInfo* model, int port );

        /**
         * Destructor
         * Will not deallocate model, that is the GUI's job
         *
         */
        ~ServerController( );

        /**
         * Starts the server networking
         *
         * @return The exit status of the server and threading. If everything worked, returns EXIT_SUCCESS, otherwise returns an error code
         */
        int spawnNetwork( );

        /**
         * Initializes the model with the information about the number of players to connect
         * and the special roles we want in the game
         *
         * @param num_clients The number of players
         * @param special_roles A vector of the special characters to use
         * @return None
         */
        int initModel( unsigned int num_clients, std::vector< avalon::special_roles_t > special_roles );

        /**
         * This method should be called on its own thread, once the controller has been set up
         * and the model has been initialized
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
