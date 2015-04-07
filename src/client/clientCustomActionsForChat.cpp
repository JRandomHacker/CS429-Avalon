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
#include <time.h>

ChatMessageSentAction::ChatMessageSentAction( std::string message_text ) : Action( "ChatMessageSent" ), message( 1, message_text, 1 ) {

}

ChatMessageSentAction::~ChatMessageSentAction( ) { }

ChatMessage ChatMessageSentAction::getMessage( ) {
	return message;
}

ChatMessageRecvAction::ChatMessageRecvAction( std::string message_text, unsigned int other_player_num ) :
	Action( "ChatMessageRecv" ), message( other_player_num, message_text, 1 ) {

}

ChatMessageRecvAction::~ChatMessageRecvAction( ) { }

ChatMessage ChatMessageRecvAction::getMessage( ) {
	return message;
}
