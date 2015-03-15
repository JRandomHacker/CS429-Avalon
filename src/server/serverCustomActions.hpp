/*
 * File of all the actions that the serverController must be able to handle
 */
#ifndef SERVERCUSTOMACTIONS_HPP
#define SERVERCUSTOMACTIONS_HPP

#include <string>

#include "action.hpp"
#include "player.hpp"
#include "settings.pb.h"

/**
 * An action that a new player has joined the game
 *
 * @class NewPlayerAction
 * @author Ryan Kerr
 * @date 2015-03-12
 */
class NewPlayerAction : public Action {
    public:

        /**
         * Public constructor
         *
         * @param player_id The id of the player that just connected
         * @param player_name The name the connecting player requested
         */
        NewPlayerAction( unsigned int player_id, std::string player_name );

        /**
         * Public destructor
         */
        virtual ~NewPlayerAction( );

        /**
         * Getter
         *
         * @return The ID of the player that just joined
         */
        unsigned int getPlayerID( );

        /**
         * Getter
         *
         * @return The name that the player requested
         */
        std::string getPlayerName( );

    private:
        unsigned int player_id;
        std::string player_name;
};

/**
 * An action that signals a state change
 *
 * @class EnterTeamSelectionAction
 * @author Ryan Kerr
 * @date 2015-03-15
 */
class EnterTeamSelectionAction : public Action {
    public:

        /**
         * Public constructor
         */
        EnterTeamSelectionAction( );

        /**
         * Public destructor
         */
        virtual ~EnterTeamSelectionAction( );
};

#endif // SERVERCUSTOMACTIONS_HPP
