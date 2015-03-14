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
    #define SOCKET int
    #include <netdb.h>
#endif

class Server {

    private:
        ServInfo* model;
        ActionHandler* queue;
        int port;
        bool clients_connected;

        struct sockaddr_in servparm;

        /*
         * Helper function to receive data and create an action
         *
         * @param recvSock The socket to recv from
         * @return None
         */
        void recvData( SOCKET recvSock );

        /*
         * Helper function to receive a custom name from the player during setup
         *
         * @param recvSock The socket to recv from
         * @return The player's requested name
         */
        std::string recvCustomName( SOCKET recvSock );

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
