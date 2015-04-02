#ifndef CHAT_MESSAGE_HPP
#define CHAT_MESSAGE_HPP

#include <string>

#include "chatmessage.pb.h"

/**
 * A class representing a chat message, either to be sent or having been received
 *
 * @class ChatMessage
 * @author Matt Hoffman
 * @date 2015-04-01
 */
class ChatMessage {
    public:
        /**
         * Public constructor
         *
         * @param player_num The id of the player who sent the message
         * @param mess_text The message's text
         */
        ChatMessage( unsigned int player_id, std::string mess_text, unsigned int time );

        /**
         * Destructor
         */
        virtual ~ChatMessage( );

        /**
         * Getter
         *
         * @return The string storing the message text
         */
        std::string getMessageText( );

        /**
         * Getter
         *
         * @return The id of the player who sent the message
         */
        unsigned int getSenderId( );

        /**
         * Getter
         *
         * @return The timestamp of when this message was received by the server
         */
        unsigned int getTimestamp( );

        /**
         * Convert to a proto buffer
         *
         * @return This object as a proto buffer
         */
        avalon::network::ChatMessage toProtoBuf( );

    private:
        unsigned int sender_id;
        std::string message_text;
        unsigned int timestamp;
};

#endif // CHAT_MESSAGE_HPP
