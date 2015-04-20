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

// EnterEndGameStateAction {
    EnterEndGameStateAction::EnterEndGameStateAction( ) : Action( "EnterEndGameState" ) { }

    EnterEndGameStateAction::~EnterEndGameStateAction( ) { }
// }

// EnterFinalGameStateAction {
    EnterFinalGameStateAction::EnterFinalGameStateAction( ) : Action( "EnterFinalGameState" ) { }

    EnterFinalGameStateAction::~EnterFinalGameStateAction( ) { }
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
    EnterTeamVoteStateAction::EnterTeamVoteStateAction( ) : Action( "EnterTeamVoteState" ) { }

    EnterTeamVoteStateAction::~EnterTeamVoteStateAction( ) { }
// }

// EnterQuestVoteStateAction {
    EnterQuestVoteStateAction::EnterQuestVoteStateAction( ) : Action( "EnterQuestVoteState" ) { }

    EnterQuestVoteStateAction::~EnterQuestVoteStateAction( ) { }
// }

// ReceiveVoteAction {
    ReceiveTeamVoteAction::ReceiveTeamVoteAction( unsigned int voter ) : Action( "ReceiveTeamVote" ) {
        this->voter = voter;
    }

    ReceiveTeamVoteAction::~ReceiveTeamVoteAction( ) { }

    unsigned int ReceiveTeamVoteAction::getVoter( ) {
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
    TeamVoteResultsAction::TeamVoteResultsAction( bool vote_result, unsigned int vote_track, std::vector< avalon::player_vote_t > votes ) : Action( "TeamVoteResults" ) {
        this->vote_result = vote_result;
        this->vote_track = vote_track;
        this->votes = votes;
    }

    TeamVoteResultsAction::~TeamVoteResultsAction( ) { }

    bool TeamVoteResultsAction::getVoteResult( ) {
        return vote_result;
    }

    unsigned int TeamVoteResultsAction::getVoteTrack( ) {
        return vote_track;
    }

    std::vector< avalon::player_vote_t > TeamVoteResultsAction::getVotes( ) {
        return votes;
    }
// }

// QuestVoteResultsAction {
    QuestVoteResultsAction::QuestVoteResultsAction( bool vote_result, unsigned int vote_track, std::vector< avalon::player_vote_t > votes ) : Action( "QuestVoteResults" ) {
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

    std::vector< avalon::player_vote_t > QuestVoteResultsAction::getVotes( ) {
        return votes;
    }
// }

// EndGameInfoAction {
    EndGameInfoAction::EndGameInfoAction( avalon::alignment_t winner, std::vector< Player > players ) : Action( "EndGameInfo" ) {
        this->winner = winner;
        this->players = players;
    }

    EndGameInfoAction::~EndGameInfoAction( ) { }

    avalon::alignment_t EndGameInfoAction::getWinner( ) {
        return winner;
    }

    std::vector< Player > EndGameInfoAction::getPlayers( ) {
        return players;
    }
// }
