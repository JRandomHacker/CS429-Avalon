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
Player::Player( std::string name, avalon::special_roles_t role, avalon::alignment_t alignment ) {

    this->name = name;
    this->role = role;
    this->alignment = alignment;
}

Player::Player( const avalon::network::Player& p ) {
	this->name = p.name( );
	this->role = static_cast< avalon::special_roles_t >( p.role( ) );
	this->alignment = static_cast< avalon::alignment_t >( p.alignment( ) );
}

// Getter for alignment
avalon::alignment_t Player::getAlignment( ) {
    return alignment;
}

// Getter for name
std::string Player::getName( ) {
    return name;
}

// Getter for role
avalon::special_roles_t Player::getRole( ) {
    return role;
}

// Setter for name
void Player::setName( std::string name ) {

    if( !name.empty() ) {
        this->name = name;
    }
}

avalon::network::Player Player::getBuf( ) {
	avalon::network::Player playerBuf;

	playerBuf.set_role( role );
	playerBuf.set_alignment( alignment );
	playerBuf.set_name( name );
	return playerBuf;
}

avalon::network::Player Player::getHiddenBuf( ) {
	avalon::network::Player playerBuf;

	playerBuf.set_role( avalon::UNKNOWN_ROLE );
	playerBuf.set_alignment( avalon::UNKNOWN_ALIGN );
	playerBuf.set_name( name );
	return playerBuf;
}
