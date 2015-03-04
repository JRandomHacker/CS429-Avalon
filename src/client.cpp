#include "client.hpp"
#include "player.hpp"
#include "globals.hpp"
#include <string>
#include <iostream>
#include <semaphore.h>
#include <pthread.h>
#include <queue>

#include "player.pb.h"

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
    if( getaddrinfo( "localhost", std::to_string( DEFAULT_PORT ).c_str(), &hints, &servinfo ) != 0 ) {
        std::cerr << "Unable to get address info" << std::endl;
        exit( EXIT_FAILURE );
    }

    if( ( sock = socket( AF_INET, SOCK_STREAM, 0 ) ) == INVALID_SOCKET ) {
        std::cerr << "Socket bind failure" << std::endl;
        exit( EXIT_FAILURE );
    }

    if( connect( sock, servinfo->ai_addr, servinfo->ai_addrlen ) != 0 ) {
        std::cerr << "Unable to connect to server" << std::endl;
        exit( EXIT_FAILURE );
    }
}

Client::~Client( ) {
	#ifdef _WIN32
        WSACleanup( );
    #endif	
}

void Client::initQueue( std::queue< Player* >* q, sem_t* qSem, pthread_mutex_t* qMutex ) {
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
	
	pthread_mutex_lock( qMutex );
	q->push( p );
	pthread_mutex_unlock( qMutex );
	sem_post( qSem );
	
	delete playerBuf;
}
