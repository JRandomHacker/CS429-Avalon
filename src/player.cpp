/*
 * Implementation of the Player class
 *
 * @file player.cpp
 * @author Ryan Kerr && Justin Koehler
 * @date 2015-02-16
 */

#include "player.hpp"
#include <string>
#include "globals.hpp"

// Constructor
Player::Player( std::string name, special_roles_t role, alignment_t alignment ) {

    this->name = name;
    this->role = role;
    this->alignment = alignment;
}

// Getter for alignment
alignment_t Player::getAlignment( ) {
    return alignment;
}

// Getter for name
std::string Player::getName( ) {
    return name;
}

// Getter for role
special_roles_t Player::getRole( ) {
    return role;
}

// Setter for name
void Player::setName( std::string name ) {

    if( !name.empty() ) {
        this->name = name;
    }
}
