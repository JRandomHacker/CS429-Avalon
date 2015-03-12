/*
 * Implementation of client class
 *
 * @file client.cpp
 * @author Ryan Kerr && Justin Koehler
 * @date 2015-03-03
 */

#include "client.hpp"
#include "player.hpp"
#include "globals.hpp"
#include <string>
#include <iostream>

#include "player.pb.h"
#include "settings.pb.h"

#ifdef _WIN32
    #include <ws2tcpip.h>
#endif

// Constructor
Client::Client( ) {
    queue = NULL;
    network_initialized = false;
}

// Destructor
Client::~Client( ) {
    // Required socket cleanup in Windows
    #ifdef _WIN32
        if( network_initialized ) {
            WSACleanup( );
        }
    #endif
}

// Allows the initialization of a queue after the client has successfully connected to the server
void Client::initQueue( ActionHandler* new_queue ) {

    if( !queue ) {
        queue = new_queue;
    }
    else {
        std::cerr << "[ CLIENT ] Attempted to add a second ActionHandler" << std::endl;
    }
}

// Sets up the clients network connection
int Client::initClient( std::string host, int port ) {

    if( network_initialized ) {
        std::cerr << "[ CLIENT ] Attempting to connect to a second server" << std::endl;
        return EXIT_NETWORK_ERROR;
    }

    // Network initialization
    struct addrinfo hints, *servinfo;
    #ifdef _WIN32
        WSADATA wsaData;
        if( WSAStartup( WINSOCK_MAGIC, &wsaData ) != 0 ) {
            std::cerr << "[ CLIENT ] Unable to magic windows" << std::endl;
            return EXIT_NETWORK_ERROR;
        }
    #endif

    // Make sure there isn't any random data in the addrinfo before setting what we want
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM; // TCP
    hints.ai_flags = 0;

    // Populate servinfo with the proper addrinfo->ai_addr to be able to connect later
    if( getaddrinfo( host.c_str( ), std::to_string( port ).c_str(), &hints, &servinfo ) != 0 ) {
        std::cerr << "[ CLIENT ] Unable to get address info" << std::endl;
        return EXIT_SOCKET_ERROR;
    }

    // Open a new socket on the system
    if( ( sock = socket( AF_INET, SOCK_STREAM, 0 ) ) == INVALID_SOCKET ) {
        std::cerr << "[ CLIENT ] Socket bind failure" << std::endl;
        return EXIT_SOCKET_ERROR;
    }

    // Connect the socket to the server
    if( connect( sock, servinfo->ai_addr, servinfo->ai_addrlen ) != 0 ) {
        std::cerr << "[ CLIENT ] Unable to connect to server" << std::endl;
        return EXIT_SOCKET_ERROR;
    }

    network_initialized = true;

    return EXIT_SUCCESS;
}

// Function to wait for another packet from the server
// Receives the information about the next message,
// then delegates to helper functions
void Client::waitForData( ) {

    if( !network_initialized ) {
        std::cerr << "[ CLIENT ] Attempting to wait for data without connecting to a server" << std::endl;
        return;
    }

    // Receive the type of protobuf we're going to be working with
    int bufType;
    int size = recv( sock, (char*) &bufType, sizeof( int ), 0 );

    // Less than equal, since on Linux an error will return 0, and on Windows an error will return -1
    if(size <= 0) {
        std::cerr << "[ CLIENT ] Network recv error";
        exit( EXIT_NETWORK_ERROR );
    }

    // Receive the length of the protobuf we're looking for
    int bufLength;
    recv( sock, (char*) &bufLength, sizeof( int ), 0 );

    // Call the relevant helper method, depending on the type of the protobuf
    switch ( bufType ){

        case avalon::network::PLAYER_BUF:
            recvPlayer( bufLength );
            break;

        case avalon::network::SETTINGS_BUF:
            recvSettings( bufLength );
            break;

        default:
            std::cerr << "[ CLIENT ] Received an unknown type of protobuf" << std::endl;
            break;
    }
}

// Helper function to receive a player protobuf
void Client::recvPlayer( int bufLength ) {

    // Receive the player
    char* playerBuf = new char[ bufLength ];
    recv( sock, playerBuf, bufLength * sizeof( char ), 0);

    // Create a new player object
    avalon::network::Player pBuf;
    pBuf.ParseFromArray( playerBuf, bufLength );
    Player* p = new Player( pBuf );

    // Add an action to the queue
    AddPlayerAction* action = new AddPlayerAction( pBuf.id(), p );
    queue->addAction( ( Action* )action );

    delete playerBuf;
}

// Helper function to receive a gamesettings protobuf
void Client::recvSettings( int bufLength ) {

    // Receive the game settings
    char* settingsBuf = new char[ bufLength ];
    recv( sock, settingsBuf, bufLength * sizeof( char ), 0);

    // Create a local copy of the GameSettings protobuf
    avalon::network::GameSettings* sBuf = new avalon::network::GameSettings( );
    sBuf->ParseFromArray( settingsBuf, bufLength );

    // Add an action to the queue
    GameSettingsAction* action = new GameSettingsAction( sBuf );
    queue->addAction( ( Action* )action );

    delete settingsBuf;
}
