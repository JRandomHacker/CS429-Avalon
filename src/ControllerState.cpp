#include "ControllerState.hpp"

ControllerState::ControllerState(std::string state_type_desc, Model* mod) :
	state_type(state_type_desc), model(mod) {

}

ControllerState::~ControllerState() {
	
}

void ControllerState::reportUnhandledAction(std::string action_type) {

}

VotingState::VotingState( Model* mod ) : ControllerState( "Voting", mod ) {
	
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

LobbyState::LobbyState( Model* mod ) : ControllerState( "Lobby", mod ) {
	
}

ControllerState* LobbyState::handleAction(Action* action_to_be_handled) {
	std::cerr << "Action being handled by lobby state" << std::endl;
	std::string action_type = action_to_be_handled->GetMessage();
	if (action_type == "GameSettings") {
		std::cerr << "GameSettings action handled by lobby state" << std::endl;
		auto action = dynamic_cast<GameSettingsAction*>(action_to_be_handled);
		avalon::network::GameSettings* sBuf = action->getSettings();
		unsigned int num_clients = sBuf->players();
		model->addData("numberOfPlayers", num_clients);
		model->addData("myID", sBuf->client());
		for ( unsigned int i = 0; i < num_clients; i++ ) {
			model->addData( std::string( "player" ) + std::to_string( i ), NULL );
		}
		model->updateData("hasGameSettings", true);
	} else if (action_type == "AddPlayer") {
		std::cerr << "AddPlayer action handled by lobby state" << std::endl;
		auto action = dynamic_cast<AddPlayerAction*>(action_to_be_handled);
		unsigned int player_number = action->getPlayerNumber();
		Player* p = action->getPlayerInfo();
		std::cerr << "AddPlayer player number: " << player_number << std::endl;
		model->updateData( std::string( "player" ) + std::to_string( player_number ), p );
	} else {
		reportUnhandledAction(action_type);
	}
	return NULL;
}
