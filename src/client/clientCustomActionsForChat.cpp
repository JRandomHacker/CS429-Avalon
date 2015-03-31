/*
 * Implementation for all of the actions the clientController must handle for chat
 *
 * @file clientCustomActionsForChat.cpp
 * @author Matt Hoffman
 * @date 2015-03-31
 */
#include "clientCustomActionsForChat.hpp"
#include "globals.hpp"

#include <string>

ChatMessageSentAction::ChatMessageSentAction( std::string message_text ) : Action( "ChatMessageSent" ), message( message_text ) {
}

ChatMessageSentAction::~ChatMessageSentAction( ) { }

std::string ChatMessageSentAction::getMessage( ) {
	return message;
}

ChatMessageRecvAction::ChatMessageRecvAction( std::string message_text, unsigned int other_player_num ) :
	Action( "ChatMessageRecv" ), message( message_text ), player_id( other_player_num ) {

}

ChatMessageRecvAction::~ChatMessageRecvAction( ) { }

std::string ChatMessageRecvAction::getMessage( ) {
	return message;
}

unsigned int ChatMessageRecvAction::getPlayerId( ) {
	return player_id;
}
