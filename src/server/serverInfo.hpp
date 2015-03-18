#ifndef SERVERINFO_HPP
#define SERVERINFO_HPP

#include "settings.pb.h"
#include "player.hpp"
#include "server.hpp"
#include <vector>
#ifdef _WIN32
    #include <windows.h>
#else
    //! Allows us to use SOCKET whether we're in Windows or Linux
    #define SOCKET int
#endif

//! Stores all the information the server needs to keep a game state
typedef struct {
    //! A pointer to the server that clients should connect to
    Server* server;
    //! The number of clients who can connect
    unsigned int num_clients;
    //! The number of times a team vote can be failed before losing
    unsigned int vote_track_length;
    //! The number of quests in the game
    unsigned int quest_track_length;
    //! Whether votes should be hidden
    bool hidden_voting;
    //! The player ID of the current leader
    unsigned int leader;
    //! The current place on the vote track
    unsigned int vote_track;
    //! A vector of all the players in the game
    std::vector< Player* > players;
    //! A vector holding the playerID and vote of anyone who's voted
    std::vector< std::pair< unsigned int, avalon::player_vote_t > > votes;
    //! A vector holding the current team selection
    std::vector< unsigned int > team;
    //! A protobuf with the game's settings
    avalon::network::GameSettings settingsBuf;
} ServInfo;

#endif // SERVERINFO_HPP
