/*
* Implementation of state class
*
* @file state.cpp
* @date 2015-03-03
*/

#include "state.hpp"

State::State() {

}

State::~State() {

}

void State::actionHandle(Action* newAct) {
    std::string actionType = newAct->GetMessage();
    std::string actionData;
    if (strcmp(actionType, "CreateGame") == 0)
      //send string of command args from Action obj to Server from GUI
      actionData = (CreateGameAction*)newAct->GetArgs();
    else if (strcmp(actionType, "JoinGame") == 0)
      //send string representing IP and port of server and player information from GUI
      actionData = (JoinGameAction*)newAct->GetServerInfo();
    else if (strcmp(actionType, "EnterLobby") == 0)
      //send game settings info for lobby from Network
      actionData = (EnterLobbyAction*)newAct->GetSettings();
    else if (strcmp(actionType, "AddPlayer") == 0)
      //send info about new player in game from Network
      actionData = (AddPlayerAction*)newAct->GetPlayerInfo();
    else
      //invalid action
}
