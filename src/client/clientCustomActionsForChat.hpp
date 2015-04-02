/*
 * File of all the Actions that the clientController must be able to handle for game chat
 */
#ifndef CLIENTCUSTOMACTIONSFORCHAT_HPP
#define CLIENTCUSTOMACTIONSFORCHAT_HPP

#include <string>

#include "action.hpp"
#include "globals.hpp"
#include "chat_message.hpp"

/**
 * An action to send a chat message to the server
 *
 * @class ChatMessageSentAction
 * @author Matt Hoffman
 * @date 2015-03-31
 */
class ChatMessageSentAction : public Action {
    public:
        /**
         * Public constructor
         *
         * @param message_text The text of the message to send
         */
        ChatMessageSentAction( std::string message_text );

        /**
         * Destructor
         */
        virtual ~ChatMessageSentAction( );

        /**
         * Gets the message to send
         *
         * @return ChatMessage What the message text is
         */
        ChatMessage getMessage( );

    private:
        //! The message being sent
        ChatMessage message;
};

/**
 * An action to receive a chat message from the server
 *
 * @class ChatMessageRecvAction
 * @author Matt Hoffman
 * @date 2015-03-31
 */
class ChatMessageRecvAction : public Action {
    public:

        /**
         * Public constructor
         *
         * @param message_text The text of the message recieved
         */
        ChatMessageRecvAction( std::string message_text, unsigned int other_player_num );

        /**
         * Destructor
         */
        virtual ~ChatMessageRecvAction( );

        /**
         * Gets what some other client is saying
         *
         * @return ChatMessage What the message is
         */
        ChatMessage getMessage( );

    private:
        //! The message received
        ChatMessage message;
};

#endif // CLIENTCUSTOMACTIONSFORCHAT_HPP
