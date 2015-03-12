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
#include "serverInfo.hpp"
#include "actionHandler.hpp"

#include "../protos/settings.pb.h"

#ifdef _WIN32
    #include <winsock2.h>
#else
    #include <netdb.h>
#endif

class Server {

    private:
        ServInfo* model;
        ActionHandler* action_queue;
        int port;

        struct sockaddr_in servparm;


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
