#include "serverCustomActions.hpp"

//Public constructors
NewPlayerAction::NewPlayerAction( unsigned int player_id ) : Action( "NewPlayer" ) {
    this->player_id = player_id;
}

NewPlayerAction::~NewPlayerAction( ) { }

//Getters
unsigned int NewPlayerAction::getPlayerID( ) {
        return player_id;
}
