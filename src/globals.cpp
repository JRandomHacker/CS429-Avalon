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
            std::cerr << "[ globals ] Invalid alignment passed into roleAlignment\n";
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
