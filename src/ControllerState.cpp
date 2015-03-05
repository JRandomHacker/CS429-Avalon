#include "ControllerState.hpp"

ControllerState::ControllerState(std::string state_type_desc, Model* mod) :
	state_type(state_type_desc), model(mod) {

}

ControllerState::~ControllerState() {
	
}

void ControllerState::reportUnhandledAction(std::string action_type) {

}

VotingState::VotingState() : ControllerState("Voting") {
	
}

ControllerState* VotingState::handleAction(Action* action_to_be_handled) {
	std::cout << "Action being handled by main screen state" << std::endl;
	std::string action_type = action_to_be_handled->GetMessage();
	if (action_type == "EnterLobby") {
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
	if (action_type == "GameSettings") {
		auto action = dynamic_cast<GameSettingsAction*>(action_to_be_handled);
		avalon::network::GameSettings* sBuf = action->getSettings();
		unsigned int num_clients = sBuf->players();
		model->addData("numberOfPlayers", num_clients);
		model->addData("myID", sBuf->client());
		for (int i = 0; i < num_clients; i++) {
			model->addData(std::string("player")+i, NULL);
		}
		model->updateData("hasGameSettings", true);
	} else if (action_type == "AddPlayer") {
		auto action = dynamic_cast<AddPlayerAction*>(action_to_be_handled);
		unsigned int player_number = action->getPlayerNumber();
		Player* p = action->getPlayerInfo();
		model->updateData(std::string("player")+player_number, p);
	} else {
		reportUnhandledAction(action_type);
	}
	return NULL;
}