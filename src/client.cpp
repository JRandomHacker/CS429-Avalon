#include "client.hpp"
#include "player.hpp"
#include "globals.hpp"
#include <string>
#include <iostream>
#include <semaphore.h>
#include <pthread.h>
#include <queue>

#include "player.pb.h"
#include "settings.pb.h"

#ifdef _WIN32
	#include <ws2tcpip.h>
#endif

Client::Client( std::string host, int port ) {
	// Network initialization
    struct addrinfo hints, *servinfo;
    #ifdef _WIN32
        WSADATA wsaData;
        if( WSAStartup( WINSOCK_MAGIC, &wsaData ) != 0 ) {
            std::cerr << "Unable to magic windows" << std::endl;
        }
    #endif

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = 0;
    if( getaddrinfo( host.c_str( ), std::to_string( port ).c_str(), &hints, &servinfo ) != 0 ) {
        std::cerr << "Unable to get address info" << std::endl;
        exit( EXIT_SOCKET_ERROR );
    }

    if( ( sock = socket( AF_INET, SOCK_STREAM, 0 ) ) == INVALID_SOCKET ) {
        std::cerr << "Socket bind failure" << std::endl;
        exit( EXIT_SOCKET_ERROR );
    }

    if( connect( sock, servinfo->ai_addr, servinfo->ai_addrlen ) != 0 ) {
        std::cerr << "Unable to connect to server" << std::endl;
        exit( EXIT_SOCKET_ERROR );
    }
}

Client::~Client( ) {
	#ifdef _WIN32
        WSACleanup( );
    #endif	
}

void Client::initQueue( std::queue< tempAction >* q, sem_t* qSem, pthread_mutex_t* qMutex ) {
	this->q = q;
	this->qSem = qSem;
	this->qMutex = qMutex;
}

void Client::waitForData( ) {
	int bufType;
	recv( sock, (char*) &bufType, sizeof( int ), 0 );
	
	int bufLength;
	recv( sock, (char*) &bufLength, sizeof( int ), 0 );
	
	switch ( bufType ){
		case avalon::network::PLAYER_BUF:
			recvPlayer( bufLength );
			break;
		case avalon::network::SETTINGS_BUF:
			recvSettings( bufLength );
			break;
		default:
			break;
	}
}

void Client::recvPlayer( int bufLength ) {
	char* playerBuf = new char[ bufLength ];
	recv( sock, playerBuf, bufLength * sizeof( char ), 0);
	
	avalon::network::Player pBuf;
	pBuf.ParseFromArray( playerBuf, bufLength );
	Player* p = new Player( pBuf );
	
	tempAction t;
	t.data = p;
	t.flag = 1; // Player* is 1
	t.playerID = pBuf.id( );
	
	pthread_mutex_lock( qMutex );
	q->push( t );
	pthread_mutex_unlock( qMutex );
	sem_post( qSem );
	
	delete playerBuf;
}

void Client::recvSettings( int bufLength ) {
	char* settingsBuf = new char[ bufLength ];
	recv( sock, settingsBuf, bufLength * sizeof( char ), 0);
	
	avalon::network::GameSettings* sBuf = new avalon::network::GameSettings( );
	sBuf->ParseFromArray( settingsBuf, bufLength );
	
	tempAction t;
	t.data = sBuf;
	t.flag = 2; // GameSettings* is 2
	
	pthread_mutex_lock( qMutex );
	q->push( t );
	pthread_mutex_unlock( qMutex );
	sem_post( qSem );
	
	delete settingsBuf;
}
