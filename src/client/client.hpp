/**
 * A class for the game client networking
 *
 * @class Client
 * @author Ryan Kerr && Justin Koehler
 * @date 2015-03-03
 */

#ifndef _CLIENT_HPP
#define _CLIENT_HPP

#include <string>
#include "player.hpp"
#include "globals.hpp"
#include "actionHandler.hpp"
#include "action.hpp"
#include "clientCustomActions.hpp"

#ifdef _WIN32
    #include <winsock2.h>
    #define WINSOCK_MAGIC 0x202
#else
    #include <netdb.h>
    #define INVALID_SOCKET -1
    // Linux SOCKETS are just ints
    #define SOCKET int
#endif

class Client {
    private:
        SOCKET sock;

        ActionHandler* queue;

        bool network_initialized;

        /*
         * Recv's an avalon::network::Player protobuf, creates an Action, and adds it to the queue
         *
         * @param bufLength The size of the protobuf that needs to be received
         * @return None
         */
        void recvPlayer( int bufLength );

        /*
         * Recv's an avalon::network::GameSettings protobuf, creates an Action, and adds it to the queue
         *
         * @param bufLength The size of the protobuf that needs to be received
         * @return None
         */
        void recvSettings( int bufLength );

    public:

        /**
         * Default contructor
         * Simply nulls out private variables
         */
        Client( );

        /**
         * Destructor for a Client
         * Performs Windows network cleanup
         */
        ~Client( );

        /**
         * Connects the client to the server with given host and port
         *
         * @param host the hostname or IP dotted quad to connect to
         * @param port the port to connect on
         * @return EXIT_SUCCESS if successful, an error code otherwise
         */
        int initClient( std::string host, int port );

        /**
         * Sets up the Clients action queue
         * If there is already an ActionHandler attached to this client,
         * prints an error and does nothing
         *
         * @param new_queue The queue to attach to the client
         * @return None
         */
        void initQueue( ActionHandler* new_queue );

        /**
         * Data processing function
         * Recv's arbitrary buffers from the server and handles them
         *
         * @return None
         */
        void waitForData( );

};
#endif
