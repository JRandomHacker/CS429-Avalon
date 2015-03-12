/*
 * Implementation of server class
 *
 * @file server.cpp
 * @author Ryan Kerr && Justin Koehler
 * @date 2015-02-16
 */

#include "server.hpp"
#include "player.hpp"
#include "globals.hpp"
#include "serverInfo.hpp"
#include "actionHandler.hpp"
#include "serverCustomActions.hpp"

#include "player.pb.h"
#include "settings.pb.h"

#include <iostream>

#ifdef _WIN32
    #include <winsock2.h>
    #define WINSOCK_MAGIC 0x202
#else
    #include <netdb.h>
    #include <sys/select.h>
    #define SOCKET int
#endif

// Initializes the server to start listening for connections
int Server::initServer( ) {

    if( port > 65536 || port < 0 )
    {
        std::cerr << "[ SERVER ] Invalid port number" << std::endl;
        return EXIT_NETWORK_ERROR;
    }
    // Windows requires some initial socket setup
    #ifdef _WIN32
        WSADATA wsaData;
        if( WSAStartup( WINSOCK_MAGIC, &wsaData ) != 0 ) {
            std::cerr << "[ SERVER ] Unable to magic windows" << std::endl;
            return EXIT_NETWORK_ERROR;
        }
    #endif

    // Create a system socket
    model->servsock = socket( AF_INET, SOCK_STREAM, 0 );

    memset( &servparm, 0, sizeof( struct sockaddr_in ) );
    servparm.sin_family = AF_INET;
    servparm.sin_port = htons( port ); // Port to listen on
    servparm.sin_addr.s_addr = htonl( INADDR_ANY ); // INET addresses to listen to

    // Bind the socket with our servparm settings
    if( bind( model->servsock, ( struct sockaddr* )( &servparm ), sizeof( struct sockaddr_in ) ) < 0 ) {
        std::cerr << "[ SERVER ] Unable to bind network socket" << std::endl;
        return EXIT_SOCKET_ERROR;
    }

    // Start listening for connections
    if( listen( model->servsock, SOMAXCONN ) < 0 ) {
        std::cerr << "[ SERVER ] Unable to listen on specified port" << std::endl;
        return EXIT_SOCKET_ERROR;
    }

    return EXIT_SUCCESS;
}

void Server::initQueue( ActionHandler* queue ) {
    if( !(this->queue) ) {
        this->queue = queue;
    } else {
        std::cerr << "[ SERVER ] Attempted to add a second ActionHandler" << std::endl;
    }
}

// Constructor
Server::Server( ServInfo* model, int port ) {

    this->model = model;
    this->port = port;
    this->queue = NULL;
    this->clients_connected = false;

}

// Destructor
Server::~Server( ) {

    // Windows requires additional socket teardown
    #ifdef _WIN32
        WSACleanup( );
    #endif
}

// TODO:
// Selects across the sockets to find one ready to read
// Should then add an action to the Queue
void Server::waitForData( ) {
    if( !clients_connected ) {
        std::cerr << "[ SERVER ] Waiting for data before connecting to clients" << std::endl;
        return;
    }

    // Set up the fd_set to select across
    fd_set clients;
    FD_ZERO( &clients );
    for( std::vector< SOCKET >::iterator it = model->sockets.begin(); it != model->sockets.end(); it++ ) {
        FD_SET( *it, &clients );
    }

    select( model->num_clients, &clients, NULL, NULL, NULL );

    // Find the sockets that had data
    for( std::vector< SOCKET >::iterator it = model->sockets.begin(); it != model->sockets.end(); it++ ) {
        if( FD_ISSET( *it, &clients ) ) {
            SOCKET sock = *it;

            // Receive the type of protobuf we're waiting for
            int bufType;
            int size = recv( sock, (char*) &bufType, sizeof( int ), 0 );

            // Make sure there wasn't a receive error
            if( size <= 0 ) {
                std::cerr << "[ SERVER ] Network recv error" << std::endl;
                exit( EXIT_NETWORK_ERROR );
            }

            // Receive the length of the protobuf we're waiting for
            int bufLength;
            recv( sock, (char*) &bufType, sizeof( int ), 0 );

            switch( bufType ) {

                case avalon::network::PLAYER_BUF:
                    std::cout << "[ SERVER ] Received a player protobuf" << std::endl;
        //            recvPlayer( bufLength );
                default:
                    std::cerr << "[ SERVER ] Received an unknown type of protobuf" << std::endl;
                    break;
            }
        }
    }
}

// Wait for all the players to connect
// Adds an action to the queue for the Controller when they do
bool Server::waitForClients( ) {

    for( unsigned int i = 0; i < model->num_clients; i++ ) {
    std::cout << i << "/" << model->num_clients << std::endl;

        model->sockets.push_back( accept( model->servsock, NULL, NULL ) );
        NewPlayerAction* action = new NewPlayerAction( i );
        queue->addAction( ( Action* )action );
    }

    clients_connected = true;
    return true;
}
