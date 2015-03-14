/*
 * File of all the Actions that the clientController must be able to handle from the network
 */
#ifndef CLIENTCUSTOMACTIONSFROMNET_HPP
#define CLIENTCUSTOMACTIONSFROMNET_HPP

#include <string>

#include "action.hpp"
#include "player.hpp"
#include "settings.pb.h"

/**
 * An action to update the game settings
 *
 * @class GameSettingsAction
 * @author Matt Hoffman
 * @date 2015-03-14
 */
class GameSettingsAction : public Action {
    public:

        /**
         * Public constructor
         *
         * @param mess_text The human readable message explaining the contents
         *     of the action
         * @param settings The string representing the game settings for the
         *     GUI to display
         */
        GameSettingsAction( avalon::network::GameSettings* settings );
        virtual ~GameSettingsAction( );

        /**
         * Getter
         *
         * @return A pointer to a protobuf containing the new game settings
         */
        avalon::network::GameSettings* getSettings( );

    private:
        avalon::network::GameSettings* game_settings;
};

/**
 * An action to add or modify a player
 *
 * @class AddPlayerAction
 * @author Matt Hoffman
 * @date 2015-03-14
 */
class AddPlayerAction : public Action {
    public:

        /**
         * Public constructor
         *
         * @param mess_text The human readable message explaining the contents
         *     of the action
         * @param player The string representing the player info for the GUI
         *     to display
         */
        AddPlayerAction( unsigned int player_number, Player* player );
        virtual ~AddPlayerAction( );

        /**
         * Getter
         *
         * @return The playerID of the player to add/modify
         */
        unsigned int getPlayerNumber( );

        /**
         * Getter
         *
         * @return A pointer to a Player object containing the information we know about the player
         */
        Player* getPlayerInfo( );

    private:
        unsigned int player_number;
        Player* player_info;
};

#endif // CLIENTCUSTOMACTIONSFROMNET_HPP
