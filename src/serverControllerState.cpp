#include "serverControllerState.hpp"

ServerControllerState::ServerControllerState( std::string state_type_desc, ServInfo* mod )
    : ControllerState(state_type_desc), model( mod ) {

}

WaitingForClientsState::WaitingForClientsState( ServInfo* mod ) : ServerControllerState( "Voting", mod ) {

}

ServerControllerState* WaitingForClientsState::handleAction(Action* action_to_be_handled) {
//    std::cout << "Action being handled by main screen state" << std::endl;
//    std::string action_type = action_to_be_handled->getMessage();
//    if (action_type == "AddClient") {
//        auto action = dynamic_cast<AddClientAction*>(action_to_be_handled);
//        client_info_temp* new_client_info = action->getClientInfo();
//        // Add the new client's information to the model
//    } else {
//        reportUnhandledAction(action_type);
//    }
//    return NULL;
}

