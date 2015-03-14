/*
 * File of all the Actions that the clientController must be able to handle from the GUI
 */
#ifndef CLIENTCUSTOMACTIONSFROMGUI_HPP
#define CLIENTCUSTOMACTIONSFROMGUI_HPP

#include <string>

#include "action.hpp"
#include "globals.hpp"

/**
 * An action to change your name
 *
 * @class SetNameAction
 * @author Matt Hoffman
 * @date 2015-03-14
 */
class SetNameAction : public Action {
    public:

        /**
         * Public constructor
         *
         * @param mess_text The human readable message explaining the contents
         *     of the action
         * @param new_name This player's new name
         */
        SetNameAction( std::string new_name );
        virtual ~SetNameAction( );

        /**
         * Gets what this client's player wants to be renamed
         *
         * @return std::String What the player's new name is
         */
        std::string getName( );

    private:
        std::string name;
};

/**
 * An action to vote on a team
 *
 * @class TeamVoteAction
 * @author Matt Hoffman
 * @date 2015-03-14
 */
class TeamVoteAction : public Action {
    public:

        /**
         * Public constructor
         *
         * @param mess_text The human readable message explaining the contents
         *     of the action
         * @param player The string representing the player info for the GUI
         *     to display
         */
        TeamVoteAction( avalon::player_vote_t new_vote );
        virtual ~TeamVoteAction( );

        /**
         * Gets what this client's player just voted
         *
         * @return avalon::player_vote_t What the player just voted, should be YES, NO, or NO_VOTE
         */
        avalon::player_vote_t getPlayerVote( );

    private:
        avalon::player_vote_t vote;
};

/**
 * An action to select a player to go on a quest
 *
 * @class SelectQuestGoerAction
 * @author Matt Hoffman
 * @date 2015-03-14
 */
class SelectQuestGoerAction : public Action {
    public:

        /**
         * Public constructor
         *
         * @param mess_text The human readable message explaining the contents
         *     of the action
         * @param is_selected If this player is being selected or deselected
         * @param selected_player_id Which player is being selected to be on or off the team
         */
        SelectQuestGoerAction( bool is_selected, unsigned int selected_player_id );
        virtual ~SelectQuestGoerAction( );

        /**
         * Gets whether this action represents a selection or a deselection
         *
         * @return bool True for selection, False for deselection
         */
        bool getSelected( );

        /**
         * Gets which player this client's player just selected or deselected
         *
         * @return unsigned int The player id of the player selected
         */
        unsigned int getSelectedPlayer( );

    private:

        bool selected;
        unsigned int selected_player;
};

/**
 * An action to finish selecting the set of players to go on a quest
 *
 * @class FinalizeTeamAction
 * @author Matt Hoffman
 * @date 2015-03-14
 */
class FinalizeTeamAction : public Action {
    public:

        /**
         * Public constructor
         *
         * @param mess_text The human readable message explaining the contents
         *     of the action
         */
        FinalizeTeamAction( );
        virtual ~FinalizeTeamAction( );
};

#endif // CLIENTCUSTOMACTIONSFROMGUI_HPP
