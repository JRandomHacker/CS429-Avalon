/**
 * A class for the game server
 *
 * @class Server
 * @author Ryan Kerr && Justin Koehler
 * @date 2015-02-16
 */

#ifndef _SERVER_HPP
#define _SERVER_HPP

#include <vector>
#include "player.hpp"
#include "globals.hpp"

#ifdef _WIN32
    #include <winsock.h>
#else
    #include <netdb.h>
#endif

class Server {

    private:
        int num_clients;
        int port;
        Player** players;

        #ifdef _WIN32
            SOCKET* sockets;
        #else
            int* sockets;
        #endif

        int servsock;
        struct sockaddr_in servparm;

        /*
         * Helper function for the constructor to actually initialize data
         */
        void initServer( );

        /*
         * Helper function for initServer to actually initialize the players array
         *
         * @param A vector of the special characters we want to use
         */
        void initPlayers( std::vector< avalon::special_roles_t > special_roles );

    public:
        /**
         * Constructor for a Server
         *
         * @param num_clients The number of clients that will connect
         * @param special_characters A list of the special characters we should include
         * @param port The port to connect to for sending/receiving data
         */
        Server( int num_clients, std::vector< avalon::special_roles_t > special_roles, int port );

        /**
         * Deconstructor for a server
         *
         */
        ~Server( );

        /**
         * A function to simply wait for initial connections
         * This function will set the correct values in the sockets array
         * and send each player their Player object
         *
         * @return True if there are now num_clients connected, False if any errors occured
         */
        bool waitForClients( );
};
#endif // _SERVER_HPP
