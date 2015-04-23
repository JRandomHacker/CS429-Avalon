/*
 * Implementation for all of the actions the clientController must handle from the GUI
 *
 * @file clientCustomActionsFromGUI.cpp
 * @author Matt Hoffman
 * @date 2015-03-14
 */
#include "clientCustomActionsFromGUI.hpp"
#include "globals.hpp"

#include <string>

SetNameAction::SetNameAction( std::string new_name ) : Action( "SetName" ), name( new_name ) {
}

SetNameAction::~SetNameAction( ) { }

std::string SetNameAction::getName( ) {
	return name;
}


TeamVoteAction::TeamVoteAction( avalon::player_vote_t new_vote ) : Action( "TeamVote" ), vote( new_vote ) {
} 

TeamVoteAction::~TeamVoteAction( ) { }

avalon::player_vote_t TeamVoteAction::getPlayerVote( ) {
	return vote;
}

QuestVoteAction::QuestVoteAction( avalon::player_vote_t new_vote ) : Action( "QuestVote" ), vote( new_vote ) {
} 

QuestVoteAction::~QuestVoteAction( ) { }

avalon::player_vote_t QuestVoteAction::getPlayerVote( ) {
	return vote;
}

SelectQuestGoerAction::SelectQuestGoerAction( bool is_selected, unsigned int selected_player_id )
	: Action( "SelectQuestGoer" ), selected( is_selected ), selected_player( selected_player_id ) {

}

SelectQuestGoerAction::~SelectQuestGoerAction( ) { }

bool SelectQuestGoerAction::getSelected( ) {
	return selected;
}

unsigned int SelectQuestGoerAction::getSelectedPlayer( ) {
	return selected_player;
}

FinalizeTeamAction::FinalizeTeamAction( ) : Action( "FinalizeTeam" ) { }

FinalizeTeamAction::~FinalizeTeamAction( ) { }

AssassinTargetSelectionAction::AssassinTargetSelectionAction( unsigned int selection_id ) : Action( "AssassinTargetSelection" ) {
    this->selection_id = selection_id;
}

AssassinTargetSelectionAction::~AssassinTargetSelectionAction( ) { }

unsigned int AssassinTargetSelectionAction::getSelectionId( ) {
	return selection_id;
}

GameEndAction::GameEndAction( ) : Action( "GameEnd" ) {
}

GameEndAction::~GameEndAction( ) { }
