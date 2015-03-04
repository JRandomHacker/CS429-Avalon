#include "CustomActions.hpp"

//Public constructors
CreateGameAction::CreateGameAction(std::string mess_text, std::string comm_args) : Action(std::string mess_text) {
    command_args = comm_args;
}

JoinGameAction::JoinGameAction(std::string mess_text, std::string serv_info) : Action(std::string mess_text) {
    server_info = serv_info;
}

EnterLobbyAction::EnterLobbyAction(std::string mess_text, std::string settings) : Action(std::string mess_text) {
    game_settings = settings;
}

AddPlayerAction::AddPlayerAction(std::string mess_text, std::string player) : Action(std::string mess_text) {
    player_info = player;
}

//Getters
std::string CreateGameAction::GetArgs() {
    return command_args;
}

std::string JoinGameAction::GetServerInfo() {
    return server_info;
}

std::string EnterLobbyAction::GetSettings() {
    return game_settings;
}

std::string AddPlayerAction::GetPlayerInfo() {
    return player_info;
}
