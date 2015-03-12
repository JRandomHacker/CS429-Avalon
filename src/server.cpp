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

#include "player.pb.h"
#include "settings.pb.h"

#include <iostream>

#ifdef _WIN32
    #include <winsock2.h>
    #define WINSOCK_MAGIC 0x202
#else
    #include <netdb.h>
#endif

// Initializes the server to start listening for connections
int Server::initServer( ) {

    if( port > 65536 || port < 0 )
    {
        std::cerr << "Invalid port number" << std::endl;
        return EXIT_NETWORK_ERROR;
    }
    // Windows requires some initial socket setup
    #ifdef _WIN32
        WSADATA wsaData;
        if( WSAStartup( WINSOCK_MAGIC, &wsaData ) != 0 ) {
            std::cerr << "Unable to magic windows" << std::endl;
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
        std::cerr << "Unable to bind network socket" << std::endl;
        return EXIT_SOCKET_ERROR;
    }

    // Start listening for connections
    if( listen( model->servsock, SOMAXCONN ) < 0 ) {
        std::cerr << "Unable to listen on specified port" << std::endl;
        return EXIT_SOCKET_ERROR;
    }

    return EXIT_SUCCESS;
}

void Server::initQueue( ActionHandler* action_queue ) {
    this->action_queue = action_queue;
}

// Constructor
Server::Server( ServInfo* model, int port ) {

    this->model = model;
    this->port = port;
    this->action_queue = NULL;

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

}

// Wait for all the players to connect
// and send them their information as they do
// TODO: THIS METHOD SHOULD SWITCH TO SENDING AN ACTION TO THE CONTROLLER
bool Server::waitForClients( ) {

    for( unsigned int i = 0; i < model->num_clients; i++ ) {
    std::cout << i << "/" << model->num_clients << std::endl;

        model->sockets.push_back( accept( model->servsock, NULL, NULL ) );
        sendStartingInfo( i );
    }

    return true;
}

// Sends one player another player's information
void Server::sendPlayer( int playerID, int destinationID, bool allInfo ) {

    avalon::network::Player playerBuf;

    if ( allInfo ) {
        playerBuf = model->players[ playerID ]->getBuf( );
    } else {
        playerBuf = model->players[ playerID ]->getHiddenBuf( );
    }

    playerBuf.set_id( playerID );
    sendProtobuf( avalon::network::PLAYER_BUF, destinationID, playerBuf.SerializeAsString( ) );
}

// Sends the beginning information when a player connects
void Server::sendStartingInfo( int playerID ) {

    model->settingsBuf.set_client( playerID );
    sendProtobuf( avalon::network::SETTINGS_BUF, playerID, model->settingsBuf.SerializeAsString( ) );

    for( int i = 0; i < playerID; i++ ) {
        sendPlayer( i, playerID, false ); // Send each currently connected player to the new player
        sendPlayer( playerID, i, false ); // Send the new player to each player already connected
    }

    sendPlayer( playerID, playerID, true ); // Send the new player their own info
}

// Sends a protobuf to a player
void Server::sendProtobuf( avalon::network::buffers_t bufType, int destinationID, std::string message ) {

    int messageSize = message.length( );

    // Windows REQUIRES that model->sockets send char* rather than void*... so we have to do some bullshit to trick it
    send( model->sockets[ destinationID ], (char*)(&bufType), sizeof( avalon::network::buffers_t ) / sizeof( char ), 0 );
    send( model->sockets[ destinationID ], (char*)(&messageSize), sizeof( int ) / sizeof( char ), 0 );
    send( model->sockets[ destinationID ], message.c_str( ), messageSize * sizeof( char ), 0 );
}
