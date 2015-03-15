#ifndef CLIENTINFO_HPP
#define CLIENTINFO_HPP

#include <vector>

#include "model.hpp"
#include "client.hpp"
#include "player.hpp"
#include "settings.pb.h"

typedef struct {
    Model* model;
    Client* client;
    unsigned int num_players;
    unsigned int my_id;
    unsigned int num_evil;
    std::vector< Player* > players;
} ClientInfo;

#endif // CLIENTINFO_HPP
