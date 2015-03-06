/**
 * A file designed to hold any structs/enums/defines that should be used across all classes
 * All values are part of namespace avalon
 *
 * @author Ryan Kerr && Justin Koehler
 * @date 2015-02-16
 * @file globals.hpp
 */

#ifndef _GLOBALS_HPP
#define _GLOBALS_HPP

#define DEFAULT_PORT 42917

#define EXIT_INVALID_PLAYERS_ERROR 10
#define EXIT_THREAD_ERROR 11
#define EXIT_EVIL_ERROR 12 // Too many evil roles compared to evil player count
#define EXIT_SOCKET_ERROR 13
#define EXIT_SERVER_NOT_FOUND 14 // Unable to find the server executable

namespace avalon {

    static const int NUM_SPECIALS = 6;

    /*! An enumerator for the possible special roles */
    enum special_roles_t {
        MERLIN,   /*! Merlin (Good) Knows evil players */
        PERCIVAL, /*! Percival (Good) Knows of Morgana and Merlin*/
        MORDRED,  /*! Mordred (Evil) Unknown to Merlin */
        MORGANA,  /*! Morgana (Evil) Known to Percival (Disguises Merlin) */
        ASSASSIN, /*! Assassin (Evil) Gets a last ditch chance to kill Merlin, making Evil win */
        OBERON,   /*! Oberon (Evil) Unknown to other Evil players */
        NONE,     /*! None Corresponds to being a generic good/evil person */
        UNKNOWN_ROLE      /*! Unknown Corresponds to another player in the game whose alignment should not be known*/
    };

    /*! An enumerator of the possible alignments */
    enum alignment_t {
        GOOD, /*! Good (Wants missions to pass) */
        EVIL, /*! Evil (Wants missions to fail) */
        UNKNOWN_ALIGN      /*! Unknown Corresponds to another player in the game whose alignment should not be known */
    };

    /**
     * A function to return the alignment of a role
     *
     * @param avalon::special_roles_t the special role to examine - Passing in the special role NONE is undefined
     * @return avalon::alignment_t The alignment of the role
     */
    alignment_t getRoleAlignment( special_roles_t role );

    /**
     * A function to get the number of evil characters based on the number of players
     *
     * @param num_players The number of players in the game
     * @return int The number of players who should be evil
     */
    unsigned int getEvilCount( int num_players );

    namespace network {
        /*! An enumerator for different protobuf types */
        enum buffers_t {
            PLAYER_BUF, /*! An avalon::network::Player protobuf */
            SETTINGS_BUF /*! An avalon::network::GameSettings protobuf */
        };
    }
}

#endif // _GLOBALS_HPP
