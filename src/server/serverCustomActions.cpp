#include "serverCustomActions.hpp"

//Public constructors
NewPlayerAction::NewPlayerAction( unsigned int player_id, std::string player_name ) : Action( "NewPlayer" ) {
    this->player_id = player_id;
    this->player_name = player_name;
}

NewPlayerAction::~NewPlayerAction( ) { }

//Getters
unsigned int NewPlayerAction::getPlayerID( ) {
        return player_id;
}

std::string NewPlayerAction::getPlayerName( ) {
    return player_name;
}

EnterTeamSelectionAction::EnterTeamSelectionAction( ) : Action( "EnterTeamSelection" ) { }

EnterTeamSelectionAction::~EnterTeamSelectionAction( ) { }
