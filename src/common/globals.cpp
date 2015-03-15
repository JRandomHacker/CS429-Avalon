/*
 * Implementation of global functions
 *
 * @author Ryan Kerr && Justin Koehler
 * @date 2015-02-16
 * @file globals.cpp
 */

#include <string>
#include <iostream>
#include "globals.hpp"

#ifndef _WIN32
    #include <unistd.h>
#else
    #include <windows.h>
#endif

// Switches a role to an alignment
avalon::alignment_t avalon::getRoleAlignment( avalon::special_roles_t role ) {

    avalon::alignment_t align;
    switch( role ) {

        case MERLIN:
        case PERCIVAL:
            align = GOOD;
            break;

        case MORDRED:
        case MORGANA:
        case ASSASSIN:
        case OBERON:
            align = EVIL;
            break;

        default:
            std::cerr << "[ globals ] Invalid alignment passed into getRoleAlignment" << std::endl;
    }

    return align;
}

// Figures out the proper number of evil players
unsigned int avalon::getEvilCount( int num_players ) {

    unsigned int evil_count = 0;
    switch( num_players ) {
        case 10:
            evil_count++;
        case 9:
        case 8:
        case 7:
            evil_count++;
        case 6:
        case 5:
            evil_count += 2;
        default:
            break;
    }

    // If we're in an invalid number of game players, just set evil_count to 1 like we've had it for testing purposes
    if( num_players && !evil_count ) {
        evil_count = 1;
    }
    return evil_count;
}
