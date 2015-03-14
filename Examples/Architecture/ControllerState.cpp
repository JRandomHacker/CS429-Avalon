#include "ControllerState.hpp"

ControllerState::ControllerState(std::string state_type_desc) :
	state_type(state_type_desc) {

}

void ControllerState::reportUnhandledAction(std::string action_type) {

}

MainScreenState::MainScreenState() : ControllerState("MainScreen") {
	
}

ControllerState* MainScreenState::handleAction(Action* action_to_be_handled) {
	std::cout << "Action being handled by main screen state" << std::endl;
	std::string action_type = action_to_be_handled->GetMessage();
	if (action_type == "CreateGame") {
		auto action = dynamic_cast<CreateGameAction*>(action_to_be_handled);
		// Run code to spawn the server with action->getArgs() as the args
	} else if (action_type == "JoinGame") {
		auto action = dynamic_cast<JoinGameAction*>(action_to_be_handled);
		// Run code to connect to the server at action->getServerInfo()
	} else if (action_type == "EnterLobby") {
		auto action = dynamic_cast<EnterLobbyAction*>(action_to_be_handled);
		// Run code to enter the lobby state with action->getSettings() as the
		// game settings
		return new LobbyState();
	} else {
		reportUnhandledAction(action_type);
	}
	return NULL;
}

LobbyState::LobbyState() : ControllerState("Lobby") {
	
}

ControllerState* LobbyState::handleAction(Action* action_to_be_handled) {
	std::cout << "Action being handled by lobby state" << std::endl;
	std::string action_type = action_to_be_handled->GetMessage();
	if (action_type == "AddPlayer") {
		auto action = dynamic_cast<AddPlayerAction*>(action_to_be_handled);
		std::cout << "Adding player: " << action->getPlayerInfo() << std::endl;
	} else {
		reportUnhandledAction(action_type);
	}
	return NULL;
}