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

#include <algorithm>
#include <random>
#include <string>
#include <iostream>

#include <netdb.h>

// Private functions
void Server::initServer( ) {

    servsock = socket( AF_INET, SOCK_STREAM, 0 );

    servparm.sin_family = AF_INET;
    servparm.sin_port = htons( port );
    servparm.sin_addr.s_addr = htonl( INADDR_ANY );

    if( bind( servsock, (struct sockaddr*)(&servparm), sizeof(struct sockaddr_in) ) < 0 ) {
        std::cerr << "Unable to bind network socket\n";
    }

    if( listen( servsock, SOMAXCONN ) < 0 ) {
        std::cerr << "Unable to listen on specified port\n";
    }
}

void Server::initPlayers( std::vector< avalon::special_roles_t > special_roles ) {

    std::vector< avalon::special_roles_t > evilChars;
    std::vector< avalon::special_roles_t > goodChars;
    std::vector< avalon::alignment_t > possibleChars;

    // Add the characters from the special_roles array to the good or evil array
    for( std::vector< avalon::special_roles_t >::iterator it = special_roles.begin(); it != special_roles.end(); it++ ) {

        if( avalon::getRoleAlignment( *it ) == avalon::EVIL ) {
            evilChars.push_back( *it );
        } else {
            goodChars.push_back( *it );
        }
    }

    // Add the possible characters to the vectors
    int numEvil = avalon::getEvilCount( num_clients );

    // possibleChars vector
    for( int i = 0; i < num_clients; i++ ) {
        if( i < numEvil ) {
            possibleChars.push_back( avalon::EVIL );
        } else {
            possibleChars.push_back( avalon::GOOD );
        }
    }

    // evilChars vector
    for( unsigned int i = 0; i < ( numEvil - evilChars.size() ); i++ ) {
        evilChars.push_back( avalon::NONE );
    }
    
    // goodChars vector
    for( unsigned int i = 0; i < ( num_clients - numEvil - goodChars.size() ); i++ ) {
        goodChars.push_back( avalon::NONE );
    }

    // Now randomize the vectors
    std::random_device rd; // Seed
    std::mt19937 rng( rd() ); // Actual generator
    std::shuffle( possibleChars.begin(), possibleChars.end(), rng );
    std::shuffle( goodChars.begin(), goodChars.end(), rng );
    std::shuffle( evilChars.begin(), evilChars.end(), rng );

    for( unsigned int i = 0; i < possibleChars.size(); i++ ) {
        avalon::alignment_t newAlign = possibleChars[ i ];
        avalon::special_roles_t newSpecial;

        if( newAlign == avalon::GOOD ) {
            newSpecial = goodChars.back();
            goodChars.pop_back();
        } else {
            newSpecial = evilChars.back();
            evilChars.pop_back();
        }

        std::string newName = "Player ";
        newName += std::to_string( i + 1 );

        players[ i ] = new Player( newName, newSpecial, newAlign );
    }
}

Server::Server( int num_clients, std::vector< avalon::special_roles_t > special_roles, int port ) {

    this->num_clients = num_clients;
    this->port = port;

    sockets = new int[ num_clients ];
    players = new Player*[ num_clients ];

    initPlayers( special_roles );
    initServer( );
}

Server::~Server( ) {

    for( int i = 0; i < num_clients; i++ ) {
        delete players[ i ];
    }
    delete[] players;
    delete sockets;
}

bool Server::waitForClients( ) {
    
    for( int i = 0; i < num_clients; i++ ) {
        int a = 42;
        sockets[ i ] = accept( servsock, NULL, NULL );
        send( sockets[ i ], &a, sizeof( int ), 0 );
    }

    return true;
}
