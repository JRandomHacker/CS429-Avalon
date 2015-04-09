#include "serverCustomActions.hpp"

// NewPlayerAction {

    NewPlayerAction::NewPlayerAction( unsigned int player_id, std::string player_name ) : Action( "NewPlayer" ) {
        this->player_id = player_id;
        this->player_name = player_name;
    }

    NewPlayerAction::~NewPlayerAction( ) { }

    unsigned int NewPlayerAction::getPlayerID( ) {
            return player_id;
    }

    std::string NewPlayerAction::getPlayerName( ) {
        return player_name;
    }
// }

// EnterTeamSelectionAction {

    EnterTeamSelectionAction::EnterTeamSelectionAction( ) : Action( "EnterTeamSelection" ) { }

    EnterTeamSelectionAction::~EnterTeamSelectionAction( ) { }
// }

// ToggleTeamMemberAction {

    ToggleTeamMemberAction::ToggleTeamMemberAction( unsigned int selector_id, unsigned int player_id ) : Action( "ToggleTeamMember" ) {
        this->selector_id = selector_id;
        this->player_id = player_id;
    }

    ToggleTeamMemberAction::~ToggleTeamMemberAction( ) { }

    unsigned int ToggleTeamMemberAction::getPlayerID( ) {
            return player_id;
    }

    unsigned int ToggleTeamMemberAction::getSelectorID( ) {
            return selector_id;
    }
// }

// ConfirmTeamSelectionAction {

    ConfirmTeamSelectionAction::ConfirmTeamSelectionAction( unsigned int selector_id ) : Action( "ConfirmTeamSelection" ) {
        this->selector_id = selector_id;
    }

    ConfirmTeamSelectionAction::~ConfirmTeamSelectionAction( ) { }

    unsigned int ConfirmTeamSelectionAction::getSelectorID( ) {
            return selector_id;
    }
// }

// TeamVoteAction {

    TeamVoteAction::TeamVoteAction( unsigned int voter, avalon::player_vote_t vote ) : Action( "TeamVote" ) {
        this->voter = voter;
        this->vote = vote;
    }

    TeamVoteAction::~TeamVoteAction( ) { }

    unsigned int TeamVoteAction::getVoter( ) {
            return voter;
    }

    avalon::player_vote_t TeamVoteAction::getVote( ) {
            return vote;
    }
// }

// QuestVoteAction {

    QuestVoteAction::QuestVoteAction( unsigned int voter, avalon::player_vote_t vote ) : Action( "QuestVote" ) {
        this->voter = voter;
        this->vote = vote;
    }

    QuestVoteAction::~QuestVoteAction( ) { }

    unsigned int QuestVoteAction::getVoter( ) {
            return voter;
    }

    avalon::player_vote_t QuestVoteAction::getVote( ) {
            return vote;
    }
// }

// ChatMessageRecvAction {

    ChatMessageRecvAction::ChatMessageRecvAction( avalon::common::ChatMessage recMessage ) :
	Action( "ChatMessageRecv" ), message( recMessage.getSenderId( ), recMessage.getMessageText( ), recMessage.getTimestamp( ) ) {

	}

	ChatMessageRecvAction::~ChatMessageRecvAction( ) { }

	avalon::common::ChatMessage ChatMessageRecvAction::getMessage( ) {
            return message;
	}
// }
