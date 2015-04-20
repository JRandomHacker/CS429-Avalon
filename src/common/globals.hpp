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

    //! The number of special characters that exist in general
    static const int NUM_SPECIALS = 6;

    /*! An enumerator for the possible special roles */
    enum special_roles_t {
        /*! Merlin (Good) Knows evil players */
        MERLIN,
        /*! Percival (Good) Knows of Morgana and Merlin*/
        PERCIVAL,
        /*! Mordred (Evil) Unknown to Merlin */
        MORDRED,
        /*! Morgana (Evil) Known to Percival (Disguises Merlin) */
        MORGANA,
        /*! Assassin (Evil) Gets a last ditch chance to kill Merlin, making Evil win */
        ASSASSIN,
        /*! Oberon (Evil) Unknown to other Evil players */
        OBERON,
        /*! None Corresponds to being a generic good/evil person */
        NONE,
        /*! Unknown Corresponds to another player in the game whose alignment should not be known*/
        UNKNOWN_ROLE
    };

    /*! An enumerator of the possible alignments */
    enum alignment_t {
        /*! Good (Wants missions to pass) */
        GOOD,
        /*! Evil (Wants missions to fail) */
        EVIL,
        /*! Unknown Corresponds to another player in the game whose alignment should not be known */
        UNKNOWN_ALIGN
    };

    /*! An enumerator of all possible voting states */
    enum player_vote_t {
        /*! A vote for the current team */
        YES,
        /*! A vote against the current team */
        NO,
        /*! This represents a player who has voted but you don't know what they voted. */
        HIDDEN,
        /*! This represents a player who has not voted */
        NO_VOTE
    };

    /**
     * A function to return the alignment of a role
     *
     * @param role the special role to examine - Passing in the special role NONE is undefined
     * @return The alignment of the role
     */
    alignment_t getRoleAlignment( special_roles_t role );

    /**
     * A function to get the number of evil characters based on the number of players
     *
     * @param num_players The number of players in the game
     * @return The number of players who should be evil
     */
    unsigned int getEvilCount( int num_players );

    namespace network {
        /*! An enumerator for different protobuf types */
        enum buffers_t {
            /*! An avalon::network::ChatMessage protobuf */
            CHAT_MSG_BUF,
            /*! An avalon::network::Player protobuf */
            PLAYER_BUF,
            /*! An avalon::network::GameSettings protobuf */
            SETTINGS_BUF,
            /*! A message corresponding to a state change to TeamSelectionState */
            ENTER_TEAM_SELECTION_BUF,
            /*! An avalon::network::TeamSelection protobuf */
            TEAM_SELECTION_BUF,
            /*! A message corresponding to a state change to VotingState */
            ENTER_TEAM_VOTE_BUF,
            /*! An avalon::network::Vote protobuf */
            TEAM_VOTE_BUF,
            /*! An avalon::network::Vote protobuf used for quest voting */
            QUEST_VOTE_BUF,
            /*! An avalon::network::VoteResults protobuf */
            TEAM_VOTE_RESULTS_BUF,
            /*! An avalon::network::QuestVoteResults protobuf */
            QUEST_VOTE_RESULTS_BUF,
            /*! A message corresponding to a state change to QuestVotingState */
            ENTER_QUEST_VOTE_BUF,
            /*! A message corresponding to a state change to EndGameState */
            ENTER_END_GAME_BUF,
            /*! A message corresponding to a state change to FinalGameState */
            ENTER_FINAL_GAME_BUF,
            /*! A message containing the winner and everyone's roles */
            END_GAME_INFO_BUF
        };
    }
}

#endif // _GLOBALS_HPP
