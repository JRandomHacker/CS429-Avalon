#include "serverControllerState.hpp"
#include "serverStateHelpers.hpp"

ServerControllerState::ServerControllerState( std::string state_type_desc, ServInfo* mod )
    : ControllerState(state_type_desc), model( mod ) {
}

WaitingForClientsState::WaitingForClientsState( ServInfo* mod ) : ServerControllerState( "Voting", mod ) {

}

WaitingForClientsState::~WaitingForClientsState( ) { }

ServerControllerState* WaitingForClientsState::handleAction( Action* action_to_be_handled ) {

    ServerStateHelper helper( model );

    std::string action_type = action_to_be_handled->getMessage();

    if( action_type == "NewPlayer" ) {
        auto action = dynamic_cast< NewPlayerAction* >( action_to_be_handled );
        unsigned int playerID = action->getPlayerID( );
        helper.sendStartingInfo( playerID );
    } else {
        reportUnhandledAction( action_type );
    }

    return NULL;
}

