#ifndef CUSTOMACTIONS_HPP
#define CUSTOMACTIONS_HPP

#include <string>

#include "action.hpp"
#include "player.hpp"
#include "settings.pb.h"

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
    GameSettingsAction(avalon::network::GameSettings* settings);
    virtual ~GameSettingsAction();

    avalon::network::GameSettings* getSettings();

private:
    avalon::network::GameSettings* game_settings;
};

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
    AddPlayerAction(unsigned int player_number, Player* player);
    virtual ~AddPlayerAction();

    unsigned int getPlayerNumber();
    Player* getPlayerInfo();

private:
    unsigned int player_number;
    Player* player_info;
};

#endif // CUSTOMACTIONS_HPP
