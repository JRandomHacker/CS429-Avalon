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
Client::Client( std::string host, int port ) {

	// Network initialization
    struct addrinfo hints, *servinfo;
    #ifdef _WIN32
        WSADATA wsaData;
        if( WSAStartup( WINSOCK_MAGIC, &wsaData ) != 0 ) {
            std::cerr << "Unable to magic windows" << std::endl;
            exit( EXIT_NETWORK_ERROR );
        }
    #endif

    // Make sure there isn't any random data in the addrinfo before setting what we want
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM; // TCP
    hints.ai_flags = 0;

    // Populate servinfo with the proper addrinfo->ai_addr to be able to connect later
    if( getaddrinfo( host.c_str( ), std::to_string( port ).c_str(), &hints, &servinfo ) != 0 ) {
        std::cerr << "Unable to get address info" << std::endl;
        exit( EXIT_SOCKET_ERROR );
    }

    // Open a new socket on the system
    if( ( sock = socket( AF_INET, SOCK_STREAM, 0 ) ) == INVALID_SOCKET ) {
        std::cerr << "Socket bind failure" << std::endl;
        exit( EXIT_SOCKET_ERROR );
    }

    // Connect the socket to the server
    if( connect( sock, servinfo->ai_addr, servinfo->ai_addrlen ) != 0 ) {
        std::cerr << "Unable to connect to server" << std::endl;
        exit( EXIT_SOCKET_ERROR );
    }
}

// Destructor
Client::~Client( ) {
    // Required socket cleanup in Windows
	#ifdef _WIN32
        WSACleanup( );
    #endif	
}

// Allows the initialization of a queue after the client has successfully connected to the server
void Client::initQueue( ActionHandler* new_queue ) {

	queue = new_queue;
}

// Function to wait for another packet from the server
// Receives the information about the next message,
// then delegates to helper functions
void Client::waitForData( ) {

    // Receive the type of protobuf we're going to be working with
	int bufType;
	int size = recv( sock, (char*) &bufType, sizeof( int ), 0 );

    // Less than equal, since on Linux an error will return 0, and on Windows an error will return -1
	if(size <= 0) {
        std::cerr << "Client network error: " << size << std::endl;
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
            std::cerr << "Received an unknown type of protobuf" << std::endl;
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
	queue->AddAction( ( Action* )action );

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
	queue->AddAction( ( Action* )action );
	
	delete settingsBuf;
}
