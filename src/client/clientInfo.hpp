/**
 * Contains the struct that is passed around between the clientController's states
 *
 * @file clientInfo.hpp
 * @author Ryan Kerr
 * @date 2015-03-14
 */
#ifndef CLIENTINFO_HPP
#define CLIENTINFO_HPP

#include <vector>

#include "model.hpp"
#include "client.hpp"
#include "player.hpp"
#include "settings.pb.h"

typedef struct {

    //! A pointer to the model that needs to be modified to change the GUI
    Model* model;
    //! A pointer to the Client that is connected to a server
    Client* client;
} ClientInfo;

#endif // CLIENTINFO_HPP
