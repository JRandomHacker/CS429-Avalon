#include "action.hpp"

// Public constructor
Action::Action(std::string mess_text) : message_text(mess_text) {

}

Action::~Action() {

}

std::string Action::getMessage() {
    return message_text;
}
