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
    std::string actionData = newAct->actionString;
    if (strcmp(actionData, "CreateGame") == 0)
      //send string of command args from Action obj to Server from GUI
    else if (strcmp(actionData, "JoinGame") == 0)
      //send string representing IP and port of server and player information from GUI
    else if (strcmp(actionData, "EnterLobby") == 0)
      //send game settings info for lobby from Network
    else if (strcmp(actionData, "AddPlayer") == 0)
      //send info about new player in game from Network
    else
      //invalid action
}
