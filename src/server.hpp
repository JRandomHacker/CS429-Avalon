/**
 * A class for the game server
 *
 * @class Server
 * @author Ryan Kerr && Justin Koehler
 * @date 2015-02-16
 */

#ifndef _SERVER_HPP
#define _SERVER_HPP

#include "serverInfo.hpp"
#include "actionHandler.hpp"

#ifdef _WIN32
    #include <winsock2.h>
#else
    #include <netdb.h>
#endif

class Server {

    private:
        ServInfo* model;
        ActionHandler* queue;
        int port;
        bool clients_connected;

        struct sockaddr_in servparm;

    public:
        /**
         * Constructor for a Server
         *
         * @param num_clients The number of clients that will connect
         * @param special_characters A list of the special characters we should include
         * @param port The port to connect to for sending/receiving data
         */
        Server( ServInfo* model, int port );

        /**
         * Deconstructor for a server
         *
         */
        ~Server( );

        /*
         * Actually initializes the servers action queue 
         *
         * @return None
         */
        void initQueue( ActionHandler* action_queue );

        /**
         * Actually initializes the servers connection
         *
         * @return EXIT_SUCCESS if the server successfully connected. An error code otherwise
         */
        int initServer( );

        /**
         * A function to simply wait for initial connections
         * This function will set the correct values in the sockets array
         * and send each player their Player object
         *
         * @return True if there are now num_clients connected, False if any errors occured
         */
        bool waitForClients( );

        /**
         * A function to select across the sockets looking for a players message
         *
         * @return True if there are now num_clients connected, False if any errors occured
         */
        void waitForData( );
};
#endif // _SERVER_HPP
