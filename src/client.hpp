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
#include <queue>
#include <semaphore.h>
#include <pthread.h>
#include "player.hpp"
#include "globals.hpp"

#ifdef _WIN32
    #include <winsock2.h>
    #define WINSOCK_MAGIC 0x202
#else
    #include <netdb.h>
    #define INVALID_SOCKET -1
#endif

class Client {
	private:
		// Windows requires the SOCKET type for sockets instead of int
		#ifdef _WIN32
			SOCKET sock;
		#else
			int sock;
		#endif
		
		std::queue< tempAction >* q;
		sem_t* qSem;
		pthread_mutex_t* qMutex;
		
		// Some sort of reference to the action queue
		
		/*
		 * Recv's an avalon::network::Player protobuf and does something with it
		 */
		void recvPlayer( int bufLength );
		
		void recvSettings( int bufLength );
		
	
	
	public:
		/**
		 * Constructor for a Client
		 * Connects socket to the server with given host and port
		 * 
		 * @param host the hostname or IP dotted quad to connect to
		 * @param port the port to connect on
		 */
		Client( std::string host, int port );
		
		/**
		 * Destructor for a Client
		 * Performs Windows network cleanup
		 */
		~Client( );
		
		void initQueue( std::queue< tempAction >* q, sem_t* qSem, pthread_mutex_t* qMutex );
		
		/**
		 * Data processing function
		 * Recv's arbitrary buffers from the server and handles them
		 * 
		 * @return None
		 */
		void waitForData( );
		
};
#endif
