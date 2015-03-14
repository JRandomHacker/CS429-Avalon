#include "CustomActions.hpp"

//Public constructors
CreateGameAction::CreateGameAction(std::string comm_args) : Action("CreateGame") {
    command_args = comm_args;
}

CreateGameAction::~CreateGameAction() {}

JoinGameAction::JoinGameAction(std::string serv_info) : Action("JoinGame") {
    server_info = serv_info;
}

JoinGameAction::~JoinGameAction() {}

EnterLobbyAction::EnterLobbyAction(std::string settings) : Action("EnterLobby") {
    game_settings = settings;
}

EnterLobbyAction::~EnterLobbyAction() {}

AddPlayerAction::AddPlayerAction(std::string player) : Action("AddPlayer") {
    player_info = player;
}

AddPlayerAction::~AddPlayerAction() {}

//Getters
std::string CreateGameAction::getArgs() {
    return command_args;
}

std::string JoinGameAction::getServerInfo() {
    return server_info;
}

std::string EnterLobbyAction::getSettings() {
    return game_settings;
}

std::string AddPlayerAction::getPlayerInfo() {
    return player_info;
}
