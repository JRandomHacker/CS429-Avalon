#include "serverCustomActions.hpp"

//Public constructors
AddClientAction::AddClientAction(client_info_temp* new_client_info) : Action("AddClient") {
    client_info = new_client_info;
}

AddClientAction::~AddClientAction() {}

//Getters
client_info_temp* AddClientAction::getClientInfo() {
	return client_info;
}
