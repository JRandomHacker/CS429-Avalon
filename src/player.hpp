/**
 * A class to store information about a player
 *
 * @class Player
 * @author Ryan Kerr && Justin Koehler
 * @date 2015-02-16
 */

#ifndef _PLAYER_HPP
#define _PLAYER_HPP
#include <string>
#include "globals.hpp"

#include "../protos/player.pb.h"

class Player {

    private:
        std::string name;
        avalon::special_roles_t role;
        avalon::alignment_t alignment;
        // VOTE HISTORY

    public:

        /**
         * Constructor for a new player
         *
         * @param name The name assigned to the player
         * @param role The special role assigned to the player, or NONE if they are not special
         * @param alignment Whether the player is on the side of good, or evil
         */
        Player( std::string name, avalon::special_roles_t role, avalon::alignment_t alignment );
        
        /**
         * Constructor for a new player from a network player
         * 
         * @param p The protobuf player from the network
         */
        Player( const avalon::network::Player& p );

        /**
         * Getter for the players alignment
         *
         * @return avalon::alignment_t The alignment of the player (good or evil)
         */
        avalon::alignment_t getAlignment();

        /**
         * Getter for the players name
         *
         * @return The name of the player
         */
        std::string getName();

        /**
         * Getter for the players role
         *
         * @return The special role of the player
         */
        avalon::special_roles_t getRole();

        /**
         * Setter for the players name
         *
         * @param name The new name for the player - Must be non-empty
         * @return None
         */
        void setName( std::string name );
        
        /**
         * Gets a protobuf representation of the Player object
         *
         * @param None
         * @return An avalon::network::Player protobuf corresponding to this player
         */
        avalon::network::Player getBuf( );

        /**
         * Gets a protobuf representation of the Player object
         * Does not reveal the player's role or alignment
         *
         * @param None
         * @return An avalon::network::Player protobuf corresponding to this player with a hidden role/alignment
         */
        avalon::network::Player getHiddenBuf( );
};
#endif // _PLAYER_HPP
