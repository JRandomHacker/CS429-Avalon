/*
 * Implementation for all of the actions the clientController must handle from the network
 *
 * @file clientCustomActionsFromNet.cpp
 * @author Matt Hoffman
 * @date 2015-03-14
 */
#include "clientCustomActionsFromNet.hpp"
#include "player.hpp"
#include "settings.pb.h"

// GameSettingsAction {
    GameSettingsAction::GameSettingsAction( avalon::network::GameSettings* settings ) : Action( "GameSettings" ) {
        game_settings = settings;
    }

    GameSettingsAction::~GameSettingsAction( ) { }

    avalon::network::GameSettings* GameSettingsAction::getSettings( ) {
        return game_settings;
    }
// }

// AddPlayerAction {
    AddPlayerAction::AddPlayerAction( unsigned int player_num, Player* player ) : Action( "AddPlayer" ) {
        player_number = player_num;
        player_info = player;
    }

    AddPlayerAction::~AddPlayerAction( ) { }

    unsigned int AddPlayerAction::getPlayerNumber( ) {
        return player_number;
    }

    Player* AddPlayerAction::getPlayerInfo( ) {
        return player_info;
    }
// }

// EnterTeamSelectionAction {
    EnterTeamSelectionAction::EnterTeamSelectionAction( unsigned int leader ) : Action( "EnterTeamSelection" ) {
        this->leader = leader;
    }

    EnterTeamSelectionAction::~EnterTeamSelectionAction( ) { }

    unsigned int EnterTeamSelectionAction::getLeader( ) {
        return leader;
    }
// }

// ModifyTeamSelectionAction {
    ModifyTeamSelectionAction::ModifyTeamSelectionAction( unsigned int selection, bool selected ) : Action( "ModifyTeamSelection" ) {
        this->selection = selection;
        this->selected = selected;
    }

    ModifyTeamSelectionAction::~ModifyTeamSelectionAction( ) { }

    unsigned int ModifyTeamSelectionAction::getSelection( ) {
        return selection;
    }

    bool ModifyTeamSelectionAction::getSelected( ) {
        return selected;
    }
// }

// EnterVoteStateAction {
    EnterVoteStateAction::EnterVoteStateAction( ) : Action( "EnterVoteState" ) { }

    EnterVoteStateAction::~EnterVoteStateAction( ) { }
// }

// EnterQuestVoteStateAction {
    EnterQuestVoteStateAction::EnterQuestVoteStateAction( ) : Action( "EnterQuestVoteState" ) { }

    EnterQuestVoteStateAction::~EnterQuestVoteStateAction( ) { }
// }

// ReceiveVoteAction {
    ReceiveVoteAction::ReceiveVoteAction( unsigned int voter ) : Action( "ReceiveVote" ) {
        this->voter = voter;
    }

    ReceiveVoteAction::~ReceiveVoteAction( ) { }

    unsigned int ReceiveVoteAction::getVoter( ) {
        return voter;
    }
// }

// ReceiveQuestVoteAction {
    ReceiveQuestVoteAction::ReceiveQuestVoteAction( unsigned int voter ) : Action( "ReceiveQuestVote" ) {
        this->voter = voter;
    }

    ReceiveQuestVoteAction::~ReceiveQuestVoteAction( ) { }

    unsigned int ReceiveQuestVoteAction::getVoter( ) {
        return voter;
    }
// }

// VoteResultsAction {
    VoteResultsAction::VoteResultsAction( bool vote_result, unsigned int vote_track, std::vector< avalon::player_vote_t >* votes ) : Action( "VoteResults" ) {
        this->vote_result = vote_result;
        this->vote_track = vote_track;
        this->votes = votes;
    }

    VoteResultsAction::~VoteResultsAction( ) { }

    bool VoteResultsAction::getVoteResult( ) {
        return vote_result;
    }

    unsigned int VoteResultsAction::getVoteTrack( ) {
        return vote_track;
    }

    std::vector< avalon::player_vote_t >* VoteResultsAction::getVotes( ) {
        return votes;
    }
// }

// QuestVoteResultsAction {
    QuestVoteResultsAction::QuestVoteResultsAction( bool vote_result, unsigned int vote_track, std::vector< avalon::player_vote_t >* votes ) : Action( "QuestVoteResults" ) {
        this->vote_result = vote_result;
        this->vote_track = vote_track;
        this->votes = votes;
    }

    QuestVoteResultsAction::~QuestVoteResultsAction( ) { }

    bool QuestVoteResultsAction::getVoteResult( ) {
        return vote_result;
    }

    unsigned int QuestVoteResultsAction::getQuestVoteTrack( ) {
        return vote_track;
    }

    std::vector< avalon::player_vote_t >* QuestVoteResultsAction::getVotes( ) {
        return votes;
    }
// }
