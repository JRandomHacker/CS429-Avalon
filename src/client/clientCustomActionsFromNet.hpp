/*
 * File of all the Actions that the clientController must be able to handle from the network
 */
#ifndef CLIENTCUSTOMACTIONSFROMNET_HPP
#define CLIENTCUSTOMACTIONSFROMNET_HPP

#include <string>

#include "action.hpp"
#include "player.hpp"
#include "settings.pb.h"

/**
 * An action to update the game settings
 *
 * @class GameSettingsAction
 * @author Matt Hoffman
 * @date 2015-03-14
 */
class GameSettingsAction : public Action {
    public:

        /**
         * Public constructor
         *
         * @param settings A GameSettings protobuf containing the game's settings
         */
        GameSettingsAction( avalon::network::GameSettings* settings );

        /**
         * Destructor
         */
        virtual ~GameSettingsAction( );

        /**
         * Getter
         *
         * @return A pointer to a protobuf containing the new game settings
         */
        avalon::network::GameSettings* getSettings( );

    private:
        avalon::network::GameSettings* game_settings;
};

/**
 * An action to add or modify a player
 *
 * @class AddPlayerAction
 * @author Matt Hoffman
 * @date 2015-03-14
 */
class AddPlayerAction : public Action {
    public:

        /**
         * Public constructor
         *
         * @param player_number The ID of the player to add
         * @param player The player object that is being added
         */
        AddPlayerAction( unsigned int player_number, Player* player );

        /**
         * Destructor
         */
        virtual ~AddPlayerAction( );

        /**
         * Getter
         *
         * @return The playerID of the player to add/modify
         */
        unsigned int getPlayerNumber( );

        /**
         * Getter
         *
         * @return A pointer to a Player object containing the information we know about the player
         */
        Player* getPlayerInfo( );

    private:
        unsigned int player_number;
        Player* player_info;
};

/**
 * An action corresponding to entering the team selection state
 *
 * @class EnterTeamSelectionAction
 * @author Ryan Kerr && Justin Koehler
 * @date 2015-03-16
 */
class EnterTeamSelectionAction : public Action {
    public:

        /**
         * Public constructor
         *
         * @param leader The current team leader
         */
        EnterTeamSelectionAction( unsigned int leader );

        /**
         * Destructor
         */
        virtual ~EnterTeamSelectionAction( );

        /**
         * Getter
         *
         * @return The current leader
         */
        unsigned int getLeader( );

    private:
        unsigned int leader;
};

/**
 * An action corresponding to receiving a team selection
 *
 * @class ModifyTeamSelectionAction
 * @author Ryan Kerr && Justin Koehler
 * @date 2015-03-16
 */
class ModifyTeamSelectionAction : public Action {
    public:

        /**
         * Public constructor
         *
         * @param selection The player who is being added or removed
         * @param selected Whether the player is being added or removed
         */
        ModifyTeamSelectionAction( unsigned int selection, bool selected );

        /**
         * Destructor
         */
        virtual ~ModifyTeamSelectionAction( );

        /**
         * Getter
         *
         * @return The current selection
         */
        unsigned int getSelection( );

        /**
         * Getter
         *
         * @return Whether the player was selected, or deselected
         */
        bool getSelected( );

    private:
        unsigned int selection;
        bool selected;
};

/**
 * An action corresponding to entering the vote state
 *
 * @class EnterVoteStateAction
 * @author Ryan Kerr && Justin Koehler
 * @date 2015-03-16
 */
class EnterTeamVoteStateAction : public Action {
    public:

        /**
         * Public constructor
         */
        EnterTeamVoteStateAction( );

        /**
         * Destructor
         */
        virtual ~EnterTeamVoteStateAction( );
};

/**
 * An action corresponding to entering the quest vote state
 *
 * @class EnterQuestVoteStateAction
 * @author Ryan Kerr && Justin Koehler
 * @date 2015-04-06
 */
class EnterQuestVoteStateAction : public Action {
    public:

        /**
         * Public constructor
         */
        EnterQuestVoteStateAction( );

        /**
         * Destructor
         */
        virtual ~EnterQuestVoteStateAction( );
};

/**
 * An action corresponding to receiving a notification that a player has voted
 *
 * @class ReceiveVoteAction
 * @author Ryan Kerr && Justin Koehler
 * @date 2015-03-16
 */
class ReceiveTeamVoteAction : public Action {
    public:

        /**
         * Public constructor
         *
         * @param voter The player that voted
         */
        ReceiveTeamVoteAction( unsigned int voter );

        /**
         * Destructor
         */
        virtual ~ReceiveTeamVoteAction( );

        /**
         * Getter
         *
         * @return The current leader
         */
        unsigned int getVoter( );

    private:
        unsigned int voter;
};

/**
 * An action corresponding to receiving a notification that a player has voted on a quest
 *
 * @class ReceiveQuestVoteAction
 * @author Ryan Kerr && Justin Koehler
 * @date 2015-04-06
 */
class ReceiveQuestVoteAction : public Action {
    public:

        /**
         * Public constructor
         *
         * @param voter The player that voted
         */
        ReceiveQuestVoteAction( unsigned int voter );

        /**
         * Destructor
         */
        virtual ~ReceiveQuestVoteAction( );

        /**
         * Getter
         *
         * @return The current leader
         */
        unsigned int getVoter( );

    private:
        unsigned int voter;
};

/**
 * An action to deal with vote results
 *
 * @class VoteResultsAction
 * @author Ryan Kerr && Justin Koehler
 * @date 2015-03-16
 */
class TeamVoteResultsAction : public Action {
    public:

        /**
         * Public constructor
         *
         * @param vote_result The result of the vote.
         * @param vote_track The new position of the vote track (0-indexed)
         * @param votes A pointer containing the votes of all players
         */
        TeamVoteResultsAction( bool vote_result, unsigned int vote_track, std::vector< avalon::player_vote_t > votes );

        /**
         * Destructor
         */
        virtual ~TeamVoteResultsAction( );

        /**
         * Getter
         *
         * @return The result of the vote.
         */
        bool getVoteResult( );

        /**
         * Getter
         *
         * @return The new position of the vote track (0-indexed)
         */
        unsigned int getVoteTrack( );

        /**
         * Getter
         *
         * @return votes A pointer containing the votes of all players
         */
        std::vector< avalon::player_vote_t > getVotes( );

    private:
        bool vote_result;
        unsigned int vote_track;
        std::vector< avalon::player_vote_t > votes;
};

/**
 * An action to deal with Quest Vote results
 *
 * @class QuestVoteResultsAction
 * @author Ryan Kerr && Justin Koehler
 * @date 2015-04-06
 */
class QuestVoteResultsAction : public Action {
    public:

        /**
         * Public constructor
         *
         * @param QuestVote_result The result of the QuestVote.
         * @param QuestVote_track The new position of the QuestVote track (0-indexed)
         * @param QuestVotes A pointer containing the QuestVotes of all players
         */
        QuestVoteResultsAction( bool vote_result, unsigned int vote_track, std::vector< avalon::player_vote_t > votes );

        /**
         * Destructor
         */
        virtual ~QuestVoteResultsAction( );

        /**
         * Getter
         *
         * @return The result of the vote.
         */
        bool getVoteResult( );

        /**
         * Getter
         *
         * @return The new position of the vote track (0-indexed)
         */
        unsigned int getQuestVoteTrack( );

        /**
         * Getter
         *
         * @return QuestVotes A pointer containing the votes of all players
         */
        std::vector< avalon::player_vote_t > getVotes( );

    private:
        bool vote_result;
        unsigned int vote_track;
        std::vector< avalon::player_vote_t > votes;
};

#endif // CLIENTCUSTOMACTIONSFROMNET_HPP
