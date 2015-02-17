/**
 * A file to implement the main method of the server executable
 *
 * @file server_main.cpp
 * @author Ryan Kerr && Justin Koehler
 * @date 2015-02-16
 */

#include <unistd.h>
#include <getopt.h>
#include <iostream>
#include <vector>
#include <pthread.h>

#include "server.hpp"
#include "globals.hpp"

// Error codes
#define EXIT_INVALID_PLAYERS 1
#define EXIT_THREAD_ERROR 2

// Globals with the options
/*
int merlin = 0;
int percival = 0;
int mordred = 0;
int morgana = 0;
int assassin = 0;
int oberon = 0;
*/
int port = 0;
int num_players = 0;

// Helper functions
std::vector< avalon::special_roles_t > parse_options( int argc, char** argv );
void* network_thread_func( void* targs );

int main( int argc, char** argv ) {
    
    std::vector< avalon::special_roles_t > selected_roles = parse_options( argc, argv );

    // Make sure we weren't given a super silly number for players
    if( num_players <= 0 ) {
        std::cerr << "--players is required, and must have a positive integer" << std::endl;
        exit( EXIT_INVALID_PLAYERS );
    }

    // Set the default port if we weren't provided one
    if( port == 0 ) {
        port = DEFAULT_PORT;
    }
    Server* serv = new Server( num_players, selected_roles, port );


    // Spawn the network listener thread
    pthread_t networkThread;
    if( pthread_create( &networkThread, NULL, &network_thread_func, &serv ) != 0 )
    {
        std::cerr << "Unable to start network thread" << std::endl;
        exit( EXIT_THREAD_ERROR );
    }

    // Just make this thread wait for now.  Later we will probably do game logic
    pthread_join( networkThread, NULL );

    exit( EXIT_SUCCESS );
}

/*
 * Function that the thread which takes care of network chatter will run
 *
 */
void* network_thread_func( void* targs ) {

    Server* server = (Server*)targs;

    server->waitForClients( );

    pthread_exit( NULL );
}

/*
 * Helper function to parse options passed into argv
 * Sets the global variables in this file accordingly
 *
 * @param argc The same argc as main
 * @param argv The same argv as main
 */
std::vector< avalon::special_roles_t > parse_options( int argc, char** argv ) {

    std::vector< avalon::special_roles_t > selected;
    struct option options[] = {
        { "players",  required_argument, NULL, 'p' },
        { "port",     required_argument, NULL, 'o' },
        { "merlin",   no_argument,       NULL, 'm' },
        { "percival", no_argument,       NULL, 'e' },
        { "mordred",  no_argument,       NULL, 'r' },
        { "morgana",  no_argument,       NULL, 'g' },
        { "assassin", no_argument,       NULL, 'a' },
        { "oberon",   no_argument,       NULL, 'b' } 
    };

    char opt;
    while( ( opt = getopt_long_only( argc, argv, "", options, NULL ) ) != -1 ) {
        switch( opt ) {
            case 'p':
                num_players = atoi( optarg );
                break;
            case 'o':
                port = atoi( optarg );
                break;
            case 'm':
                selected.push_back( avalon::MERLIN );
                break;
            case 'e':
                selected.push_back( avalon::PERCIVAL );
                break;
            case 'r':
                selected.push_back( avalon::MORDRED );
                break;
            case 'g':
                selected.push_back( avalon::MORGANA );
                break;
            case 'a':
                selected.push_back( avalon::ASSASSIN );
                break;
            case 'b':
                selected.push_back( avalon::OBERON );
                break;
            default:
                break;
        }
    }

    return selected;
}
