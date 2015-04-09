#include "chat_message.hpp"

namespace avalon {
namespace common {

ChatMessage::ChatMessage( unsigned int player_id, std::string mess_text, unsigned int time )
	: sender_id( player_id ), message_text( mess_text ), timestamp( time ) {

}

ChatMessage::ChatMessage( avalon::network::ChatMessage proto_buf ) 
	: sender_id( proto_buf.sender_id( ) ),
	message_text( proto_buf.message_text( ) ),
	timestamp( proto_buf.timestamp( ) ) {

}

ChatMessage::~ChatMessage( ) {

}

std::string ChatMessage::getMessageText( ) {
	return message_text;
}

unsigned int ChatMessage::getSenderId( ) {
	return sender_id;
}

unsigned int ChatMessage::getTimestamp( ) {
	return timestamp;
}

avalon::network::ChatMessage ChatMessage::toProtoBuf( ) {
	avalon::network::ChatMessage buf;
	buf.set_sender_id( sender_id );
	buf.set_message_text( message_text );
	buf.set_timestamp( timestamp );
	return buf;
}

} // namespace common
} // namespace avalon
