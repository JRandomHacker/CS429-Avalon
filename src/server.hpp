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

#include "../protos/settings.pb.h"

#ifdef _WIN32
    #include <winsock2.h>
#else
    #include <netdb.h>
#endif

class Server {

    private:
        unsigned int num_clients;
        int port;
        Player** players;

        // A place to store the settings on creation to be able to send them to
        // players as they connect
        avalon::network::GameSettings settingsBuf;

        // Windows requires the SOCKET type for sockets instead of int
        #ifdef _WIN32
            SOCKET* sockets;
            SOCKET servsock;
        #else
            int* sockets;
            int servsock;
        #endif

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

        /*
         * Helper function to send a player to another player
         *
         * @param playerID The ID of the player to be sent
         * @param destintationID The ID of the player to send the player to
         * @param allInfo Whether the player should know the role and alignment of the sent player
         * @return None
         */
        void sendPlayer( int playerID, int destinationID, bool allInfo );

        /*
         * Helper function to send everything a player needs upon connecting to the server
         * Sends settings, their player object, and all currently connected players
         * Also sends the new player to all currently connected players
         *
         * @param playerID the ID of the player that is connecting
         * @return None
         */
        void sendStartingInfo( int playerID );

        /*
         * Helper function to send a protobuf
         *
         * @param bufType The type of the buffer that is being sent
         * @param destinationID The ID of the player that the buffer should be sent to
         * @param message The protobuf to be sent, already serialized as a string (Using protobuf's method)
         * @return None
         */
        void sendProtobuf( avalon::network::buffers_t bufType, int destinationID, std::string message );

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
