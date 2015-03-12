#ifndef SERVERINFO_HPP
#define SERVERINFO_HPP

#include "../protos/settings.pb.h"
#include "player.hpp"
#include <vector>
#ifdef _WIN32
    #include <windows.h>
#else
    #define SOCKET int
#endif

typedef struct {
    SOCKET servsock;
    unsigned int num_clients;
    std::vector< Player* > players;
    std::vector< SOCKET > sockets;
    std::vector< bool > votes;
    avalon::network::GameSettings settingsBuf;
} ServInfo;

#endif // SERVERINFO_HPP
