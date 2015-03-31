/*
 * File of all the Actions that the clientController must be able to handle for game chat
 */
#ifndef CLIENTCUSTOMACTIONSFORCHAT_HPP
#define CLIENTCUSTOMACTIONSFORCHAT_HPP

#include <string>

#include "action.hpp"
#include "globals.hpp"

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
         * Gets what this client's player is saying
         *
         * @return std::string What the message text is
         */
        std::string getMessage( );

    private:
        //! The message being sent
        std::string message;
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
         * @return std::string What the message text is
         */
        std::string getMessage( );

        /**
         * Gets who sent this message
         *
         * @return unsigned int Who sent the message
         */
        unsigned int getPlayerId( );

    private:
        //! The message received
        std::string message;

        //! The player id of the player who sent the message
        unsigned int player_id;
};

#endif // CLIENTCUSTOMACTIONSFROMGUI_HPP
