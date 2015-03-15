#ifndef SERVERINFO_HPP
#define SERVERINFO_HPP

#include "settings.pb.h"
#include "player.hpp"
#include "server.hpp"
#include <vector>
#ifdef _WIN32
    #include <windows.h>
#else
    #define SOCKET int
#endif

typedef struct {
    //! A pointer to the server that clients should connect to
    Server* server;
    //! The number of clients who can connect
    unsigned int num_clients;
    //! The player ID of the current leader
    unsigned int leader;
    //! A vector of all the players in the game
    std::vector< Player* > players;
    //! A vector holding the playerID and vote of anyone who's voted
    std::vector< std::pair< unsigned int, bool > > votes;
    //! A vector holding the current team selection
    std::vector< unsigned int > team;
    //! A protobuf with the game's settings
    avalon::network::GameSettings settingsBuf;
} ServInfo;

#endif // SERVERINFO_HPP
