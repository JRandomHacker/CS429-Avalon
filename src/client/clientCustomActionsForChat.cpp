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

ChatMessageSentAction::ChatMessageSentAction( avalon::common::ChatMessage mess ) : Action( "ChatMessageSent" ), message( mess ) {

}

ChatMessageSentAction::~ChatMessageSentAction( ) { }

avalon::common::ChatMessage ChatMessageSentAction::getMessage( ) {
	return message;
}

ChatMessageRecvAction::ChatMessageRecvAction( avalon::common::ChatMessage mess ) :
	Action( "ChatMessageRecv" ), message( mess ) {

}

ChatMessageRecvAction::~ChatMessageRecvAction( ) { }

avalon::common::ChatMessage ChatMessageRecvAction::getMessage( ) {
	return message;
}
