/*
 * Implementation for all of the actions the clientController must handle from the network
 *
 * @file clientCustomActionsFromNet.cpp
 * @author Matt Hoffman
 * @date 2015-03-14
 */
#include "clientCustomActionsFromNet.hpp"
#include "player.hpp"
#include "settings.pb.h"

// GameSettingsAction {
    GameSettingsAction::GameSettingsAction( avalon::network::GameSettings* settings ) : Action( "GameSettings" ) {
        game_settings = settings;
    }

    GameSettingsAction::~GameSettingsAction( ) { }

    avalon::network::GameSettings* GameSettingsAction::getSettings( ) {
        return game_settings;
    }
// }

// AddPlayerAction {
    AddPlayerAction::AddPlayerAction( unsigned int player_num, Player* player ) : Action( "AddPlayer" ) {
        player_number = player_num;
        player_info = player;
    }

    AddPlayerAction::~AddPlayerAction( ) { }

    unsigned int AddPlayerAction::getPlayerNumber( ) {
        return player_number;
    }

    Player* AddPlayerAction::getPlayerInfo( ) {
        return player_info;
    }
// }

// EnterTeamSelectionAction {
    EnterTeamSelectionAction::EnterTeamSelectionAction( unsigned int leader ) : Action( "EnterTeamSelection" ) {
        this->leader = leader;
    }

    EnterTeamSelectionAction::~EnterTeamSelectionAction( ) { }

    unsigned int EnterTeamSelectionAction::getLeader( ) {
        return leader;
    }
// }

// EnterVoteStateAction {
    EnterVoteStateAction::EnterVoteStateAction( ) : Action( "EnterVoteState" ) { }

    EnterVoteStateAction::~EnterVoteStateAction( ) { }
// }
