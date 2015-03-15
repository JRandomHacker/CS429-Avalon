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
    Server* server;
    unsigned int num_clients;
    unsigned int leader;
    std::vector< Player* > players;
    std::vector< bool > votes;
    avalon::network::GameSettings settingsBuf;
} ServInfo;

#endif // SERVERINFO_HPP
