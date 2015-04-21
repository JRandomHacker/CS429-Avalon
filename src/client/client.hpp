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
#include "clientCustomActionsFromNet.hpp"
#include "clientCustomActionsForChat.hpp"

#ifdef _WIN32
    #include <winsock2.h>
    //! A magic number that needs to be passed for Windows socket initialization
    #define WINSOCK_MAGIC 0x202
#else
    #include <netdb.h>
    //! In windows, socket returns a specific value, INVALID_SOCKET, on failure. This allows us to use the same code in Linux
    #define INVALID_SOCKET -1
    //! Allows us to use the SOCKET type in both Windows and Linux
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
        void recvPlayer( unsigned int bufLength );

        /*
         * Recv's an avalon::network::Player protobuf corresponding to the player the assassin killed
         *
         * @param bufLength The size of the protobuf that needs to be received
         * @return None
         */
        void recvAssassinSelection( unsigned int bufLength );

        /*
         * Recv's an avalon::network::ChatMessage protobuf, creates an Action, and adds it to the queue
         *
         * @param bufLength The size of the protobuf that needs to be received
         * @return None
         */
        void recvMessage( unsigned int bufLength );

        /*
         * Recv's an avalon::network::Vote protobuf for a team vote, creates an Action, and adds it to the queue
         *
         * @param bufLength The size of the protobuf that needs to be received
         * @return None
         */
        void recvTeamVote( unsigned int bufLength );

        /*
         * Recv's an avalon::network::Vote protobuf for a quet vote, creates an Action, and adds it to the queue
         *
         * @param bufLength The size of the protobuf that needs to be received
         * @return None
         */
        void recvQuestVote( unsigned int bufLength );

        /*
         * Recv's an avalon::network::EndGame protobuf and deals with it
         *
         * @param bufLength The size of the protobuf that needs to be received
         * @return None
         */
        void recvEndGameInfo( unsigned int bufLength );

        /*
         * Recv's an avalon::network::VoteResults protobuf, creates an Action, and adds it to the queue
         *
         * @param bufLength The size of the protobuf that needs to be received
         * @return None
         */
        void recvTeamVoteResults( unsigned int bufLength );
        
        /*
         * Recv's an avalon::network::QuestVoteResults protobuf, creates an Action, and adds it to the queue
         *
         * @param bufLength The size of the protobuf that needs to be received
         * @return None
         */
        void recvQuestVoteResults( unsigned int bufLength );

        /*
         * Recv's an avalon::network::TeamSelection protobuf, creates an Action, and adds it to the queue
         *
         * @param bufLength The size of the protobuf that needs to be received
         * @return None
         */
        void recvTeamSelection( unsigned int bufLength );

        /*
         * Processes a state change method and creates the appropriate action
         *
         * @param bufLength The size of the protobuf that needs to be received
         * @param randomness Maybe data... maybe not... depends on our state
         * @return None
         */
        void recvStateChange( int bufType, unsigned int randomness  );

        /*
         * Recv's an avalon::network::GameSettings protobuf, creates an Action, and adds it to the queue
         *
         * @param bufLength The size of the protobuf that needs to be received
         * @return None
         */
        void recvSettings( unsigned int bufLength );

        /*
         * Adds an action to the queue telling the controller to throw up its hands
         *
         * @return None
         */
        void addShutdownAction( );

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
         * @return True if successfully received data, false otherwise
         */
        bool waitForData( );

        /**
         * Function to send a protobuf to the server the client is connected to
         *
         * @param bufType The type of buffer that is being sent
         * @param message The protobuf to be sent, already serialized as a string (Using protobuf method)
         *
         * @return None
         */
        virtual void sendProtobuf( avalon::network::buffers_t bufType, std::string message );

};
#endif
