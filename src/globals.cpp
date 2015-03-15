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

    if( num_players ) {
        return 1;
    }
    return 0;
}

void avalon::sleep( int seconds ) {
    #ifdef _WIN32
        Sleep( seconds * 1000 );
    #else
        sleep( seconds );
    #endif
}