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

#include "serverController.hpp"
#include "globals.hpp"


/**
 * Helper function to parse the options passed into the program
 *
 * @param argc The number of arguments passed in
 * @param argv An array of the arguments passed in
 * @return A vector containing all the chosen special roles
 */
ServerSettings parse_options( int argc, char** argv );

/**
 * Main function for the server executable
 *
 * @param argc The number of arguments passed in
 * @param argv An array of the arguments passed in
 * @return EXIT_SUCCESS on a normal exit, an error otherwise
 */
int main( int argc, char** argv ) {

    int retval;

    ServerSettings server_settings = parse_options( argc, argv );

    // Make sure we weren't given a super silly number for players
    if( server_settings.num_players <= 0 ) {
        std::cerr << "--players is required, and must have a positive integer" << std::endl;
        exit( EXIT_INVALID_PLAYERS_ERROR );
    }

    // Set the default port if we weren't provided one
    if( server_settings.port == 0 ) {
        server_settings.port = DEFAULT_PORT;
    }

    server_settings.game_settings.loadFromFile( "config/settings.lua", server_settings.num_players );

    ServInfo* model = new ServInfo;
    ServerController* controller = new ServerController( model, server_settings );
    if( ( retval = controller->initModel( server_settings ) ) != EXIT_SUCCESS ) {
        exit( retval );
    }
    if( ( retval = controller->spawnNetwork( ) ) != EXIT_SUCCESS ) {
        exit( retval );
    }

    controller->processActions( );

    delete controller;
    delete model;

    exit( EXIT_SUCCESS );
}

/*
 * Helper function to parse options passed into argv
 * returns a server settings object with the appropriate stuff
 *
 * @param argc The same argc as main
 * @param argv The same argv as main
 * @param avalon_settings The avalon settings loaded in from the ini file
 */
ServerSettings parse_options( int argc, char** argv ) {
    ServerSettings settings;

    struct option options[] = {
        { "players",  required_argument, NULL, 'p' },
        { "port",     required_argument, NULL, 'o' },
        { "hidden",   no_argument,       NULL, 'h' },
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
                settings.num_players = atoi( optarg );
                break;
            case 'o':
                settings.port = atoi( optarg );
                break;
            case 'h':
                settings.hidden_votes = true;
                break;
            case 'm':
                settings.selected_roles.push_back( avalon::MERLIN );
                break;
            case 'e':
                settings.selected_roles.push_back( avalon::PERCIVAL );
                break;
            case 'r':
                settings.selected_roles.push_back( avalon::MORDRED );
                break;
            case 'g':
                settings.selected_roles.push_back( avalon::MORGANA );
                break;
            case 'a':
                settings.selected_roles.push_back( avalon::ASSASSIN );
                break;
            case 'b':
                settings.selected_roles.push_back( avalon::OBERON );
                break;
            default:
                break;
        }
    }

    return settings;
}
