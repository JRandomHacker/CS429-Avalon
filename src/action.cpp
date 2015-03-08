#include "Action.hpp"

// Public constructor  
Action::Action(std::string mess_text) : message_text(mess_text) {

}

Action::~Action() {
	
}

std::string Action::GetMessage() {
	return message_text;
}