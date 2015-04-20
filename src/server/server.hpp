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

#include "actionHandler.hpp"
#include "globals.hpp"
#include "chat_message.hpp"

#ifdef _WIN32
    #include <winsock2.h>
#else
    //! Allows us to use the SOCKET type in both Windows and Linux
    #define SOCKET int
    #include <netdb.h>
#endif

class Server {

    private:
        SOCKET servsock;
        std::vector< SOCKET > sockets;
        ActionHandler* queue;
        int port;
        unsigned int num_clients;
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
         * Helper function to get the TeamSelection protobuf and create an action
         *
         * @param recvSock The socket to recv from
         * @param bufLength The length of the protobuf
         * @return None
         */
        void recvTeamSelection( SOCKET recvSock, int bufLength );

        /*
         * Helper function to deal with an end game buffer
         *
         * @param recvSock The socket to recv from
         * @param bufLength The length of the protobuf
         * @return None
         */
        void recvConfirmEnd( SOCKET recvSock, int bufLength );

        /*
         * Helper function to get the ChatMessage protobuf and create an action
         *
         * @param recvSock The socket to recv from
         * @param bufLength The length of the protobuf
         * @return None
         */
        void recvMessage( SOCKET recvSock, int bufLength );

        /*
         * Helper function to get the Vote protobuf for a team vote and create an action
         *
         * @param recvSock The socket to recv from
         * @param bufLength The length of the protobuf
         * @return None
         */
        void recvTeamVote( SOCKET recvSock, int bufLength );

        /*
         * Helper function to get the Vote protobuf for a quest vote and create an action
         *
         * @param recvSock The socket to recv from
         * @param bufLength The length of the protobuf
         * @return None
         */
        void recvQuestVote( SOCKET recvSock, int bufLength );

        /*
         * Helper function to receive a custom name from the player during setup
         *
         * @param recvSock The socket to recv from
         * @return The player's requested name
         */
        std::string recvCustomName( SOCKET recvSock );

        /*
         * Helper function to convert a SOCKET to an int in Windows
         *
         * @param recvSock The socket we want an int of
         * @return The location of the SOCKET in the sockets vector
         */
        unsigned int getIdFromSocket( SOCKET recvSock );

    public:

        /**
         * Constructor for a Server
         *
         * @param port The port to connect to for sending/receiving data
         */
        Server( int port );

        /**
         * Deconstructor for a server
         *
         */
        ~Server( );

        /**
         * Actually initializes the servers action queue
         *
         * @param action_queue The action queue we should put actions into
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
         * @param num_clients The number of clients that we expect in the game
         * @return True if there are now num_clients connected, False if any errors occured
         */
        bool waitForClients( unsigned int num_clients );

        /**
         * A function to select across the sockets looking for a players message
         *
         * @return True if there are now num_clients connected, False if any errors occured
         */
        void waitForData( );

        /**
         * Helper function to send a protobuf
         *
         * @param bufType The type of the buffer that is being sent
         * @param destinationID The ID of the player that the buffer should be sent to
         * @param message The protobuf to be sent, already serialized as a string (Using protobuf's method)
         * @return None
         */
        virtual void sendProtobuf( avalon::network::buffers_t bufType, unsigned int destinationID, std::string message );

        /**
         * Helper function to tell the clients to change states
         *
         * @param bufType The type of the buffer that is being sent
         * @param randomness Possibly some data... or not... who knows
         * @return None
         */
        virtual void broadcastStateChange( avalon::network::buffers_t bufType, unsigned int randomness );

};
#endif // _SERVER_HPP
