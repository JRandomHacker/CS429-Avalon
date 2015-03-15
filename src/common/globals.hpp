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

//! The default port for the server to listen on and the client to connect to
#define DEFAULT_PORT 42917 
//! Exit status if there are too many, or too few, players
#define EXIT_INVALID_PLAYERS_ERROR 10
//! Exit status if we're unable to spawn a thread
#define EXIT_THREAD_ERROR 11
//! Exit status if there are more special players than players
#define EXIT_SPECIAL_ERROR 12
//! Exit status if there are more evil special players than evil players
#define EXIT_EVIL_ERROR 13
//! Exit status if there are more good special players than good players
#define EXIT_GOOD_ERROR 14
//! Exit status if a socket cannot be bound or connected to
#define EXIT_SOCKET_ERROR 15
//! Exit status if the server executable cannot be found
#define EXIT_SERVER_NOT_FOUND 16
//! Exit status if we lose a connection
#define EXIT_NETWORK_ERROR 17
//! Exit status in Windows if the exit code of the server is larger than an integer
#define UNKNOWN_ERROR 42

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

    /*! An enumerator of all possible voting states */
    enum player_vote_t {
        YES, /* A vote for the current team */
        NO, /* A vote against the current team */
        HIDDEN, /*! This represents a player who has voted but you don't know what they voted. */
        NO_VOTE,  /* This represents a player who has not voted */
        UNKNOWN_VOTE  /* This represents knowing nothing about whether a player has voted */
    };

    namespace network {
        /*! An enumerator for different protobuf types */
        enum buffers_t {
            PLAYER_BUF, /*! An avalon::network::Player protobuf */
            SETTINGS_BUF, /*! An avalon::network::GameSettings protobuf */
            TEAM_SELECTION_BUF, /*! An avalon::network::TeamSelection protobuf */
            VOTE_BUF /*! An avalon::network::Vote protobuf */
        };
    }
}

#endif // _GLOBALS_HPP
