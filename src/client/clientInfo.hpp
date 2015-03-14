#ifndef CLIENTINFO_HPP
#define CLIENTINFO_HPP

#include "model.hpp"
#include "client.hpp"
#include "settings.pb.h"

typedef struct {
    Model* model;
    Client* client;
    unsigned int num_players;
    unsigned int my_id;
    avalon::network::GameSettings settingsBuf;
} ClientInfo;

#endif // CLIENTINFO_HPP
