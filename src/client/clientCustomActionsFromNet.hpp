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
         * @param settings A GameSettings protobuf containing the game's settings
         */
        GameSettingsAction( avalon::network::GameSettings* settings );

        /**
         * Destructor
         */
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
         * @param player_number The ID of the player to add
         * @param player The player object that is being added
         */
        AddPlayerAction( unsigned int player_number, Player* player );

        /**
         * Destructor
         */
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

/**
 * An action corresponding to entering the team selection state
 *
 * @class EnterTeamSelectionAction
 * @author Ryan Kerr && Justin Koehler
 * @date 2015-03-16
 */
class EnterTeamSelectionAction : public Action {
    public:

        /**
         * Public constructor
         *
         * @param leader The current team leader
         */
        EnterTeamSelectionAction( unsigned int leader );

        /**
         * Destructor
         */
        virtual ~EnterTeamSelectionAction( );

        /**
         * Getter
         *
         * @return The current leader
         */
        unsigned int getLeader( );

    private:
        unsigned int leader;
};

/**
 * An action corresponding to entering the vote state
 *
 * @class EnterVoteStateAction
 * @author Ryan Kerr && Justin Koehler
 * @date 2015-03-16
 */
class EnterVoteStateAction : public Action {
    public:

        /**
         * Public constructor
         */
        EnterVoteStateAction( );

        /**
         * Destructor
         */
        virtual ~EnterVoteStateAction( );
};

#endif // CLIENTCUSTOMACTIONSFROMNET_HPP
