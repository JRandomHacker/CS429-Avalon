/*
 * File of all the actions that the serverController must be able to handle
 */
#ifndef SERVERCUSTOMACTIONS_HPP
#define SERVERCUSTOMACTIONS_HPP

#include <string>

#include "action.hpp"
#include "player.hpp"
#include "chat_message.hpp"
#include "settings.pb.h"

/**
 * An action that a new player has joined the game
 *
 * @class NewPlayerAction
 * @author Ryan Kerr
 * @date 2015-03-12
 */
class NewPlayerAction : public Action {
    public:

        /**
         * Public constructor
         *
         * @param player_id The id of the player that just connected
         * @param player_name The name the connecting player requested
         */
        NewPlayerAction( unsigned int player_id, std::string player_name );

        /**
         * Public destructor
         */
        virtual ~NewPlayerAction( );

        /**
         * Getter
         *
         * @return The ID of the player that just joined
         */
        unsigned int getPlayerID( );

        /**
         * Getter
         *
         * @return The name that the player requested
         */
        std::string getPlayerName( );

    private:
        unsigned int player_id;
        std::string player_name;
};

/**
 * An action that signals a state change
 *
 * @class EnterTeamSelectionAction
 * @author Ryan Kerr
 * @date 2015-03-15
 */
class EnterTeamSelectionAction : public Action {
    public:

        /**
         * Public constructor
         */
        EnterTeamSelectionAction( );

        /**
         * Public destructor
         */
        virtual ~EnterTeamSelectionAction( );
};

/**
 * An action for adding or removing a team member from the current team selection
 *
 * @class ToggleTeamMemberAction
 * @author Ryan Kerr
 * @date 2015-03-15
 */
class ToggleTeamMemberAction : public Action {
    public:

        /**
         * Public constructor
         *
         * @param selector_id The ID of the player attempting to modify team selection
         * @param player_id The ID of the player to select or remove
         */
        ToggleTeamMemberAction( unsigned int selector_id, unsigned int player_id );

        /**
         * Public destructor
         */
        virtual ~ToggleTeamMemberAction( );

        /**
         * Getter
         *
         * @return The ID of the player attempting to modify team selection
         */
        unsigned int getSelectorID( );

        /**
         * Getter
         *
         * @return The ID of the player that should be added/removed
         */
        unsigned int getPlayerID( );

    private:
        unsigned int selector_id;
        unsigned int player_id;
};

/**
 * An action for confirming the team selection and moving to the voting state
 *
 * @class ConfirmTeamSelectionAction
 * @author Ryan Kerr
 * @date 2015-03-15
 */
class ConfirmTeamSelectionAction : public Action {
    public:

        /**
         * Public constructor
         *
         * @param selector_id The ID of the player attempting to confirm
         */
        ConfirmTeamSelectionAction( unsigned int selector_id );

        /**
         * Public destructor
         */
        virtual ~ConfirmTeamSelectionAction( );

        /**
         * Getter
         *
         * @return The ID of the player attempting to modify team selection
         */
        unsigned int getSelectorID( );

    private:
        unsigned int selector_id;
};

/**
 * An action for receiving a vote
 *
 * @class ConfirmTeamSelectionAction
 * @author Ryan Kerr && Justin Koehler
 * @date 2015-03-16
 */
class TeamVoteAction : public Action {
    public:

        /**
         * Public constructor
         *
         * @param voter The ID of the player who sent the vote
         * @param vote vote that the player sent
         */
        TeamVoteAction( unsigned int voter, avalon::player_vote_t vote );

        /**
         * Public destructor
         */
        virtual ~TeamVoteAction( );

        /**
         * Getter
         *
         * @return The ID of the player attempting to vote
         */
        unsigned int getVoter( );

        /**
         * Getter
         *
         * @return The vote of the voting player
         */
        avalon::player_vote_t getVote( );

    private:
        unsigned int voter;
        avalon::player_vote_t vote;
};

/**
 * An action for receiving a quest vote
 *
 * @class ConfirmTeamSelectionAction
 * @author Ryan Kerr && Justin Koehler
 * @date 2015-04-06
 */
class QuestVoteAction : public Action {
    public:

        /**
         * Public constructor
         *
         * @param voter The ID of the player who sent the vote
         * @param vote vote that the player sent
         */
        QuestVoteAction( unsigned int voter, avalon::player_vote_t vote );

        /**
         * Public destructor
         */
        virtual ~QuestVoteAction( );

        /**
         * Getter
         *
         * @return The ID of the player attempting to vote
         */
        unsigned int getVoter( );

        /**
         * Getter
         *
         * @return The vote of the voting player
         */
        avalon::player_vote_t getVote( );

    private:
        unsigned int voter;
        avalon::player_vote_t vote;
};

/**
 * An action to receive a chat message from a client
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
         * @param recMessage The ChatMessage received from the client
         */
        ChatMessageRecvAction( ChatMessage recMessage );

        /**
         * Destructor
         */
        virtual ~ChatMessageRecvAction( );

        /**
         * Gets what some client is saying
         *
         * @return ChatMessage What the message is
         */
        avalon::common::ChatMessage getMessage( );

    private:
        //! The message received
        avalon::common::ChatMessage message;
};

#endif // SERVERCUSTOMACTIONS_HPP
