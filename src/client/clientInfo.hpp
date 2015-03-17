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
    //! The number of players in the game
    unsigned int num_players;
    //! My player number
    unsigned int my_id;
    //! The number of evil characters in the game
    unsigned int num_evil;
    //! The current leader - Person who proposes team
    unsigned int leader;
    //! A vector of all the players in the game
    std::vector< Player* > players;

} ClientInfo;

#endif // CLIENTINFO_HPP
