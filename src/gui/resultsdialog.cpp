#include "resultsdialog.hpp"
#include "ui_resultsdialog.h"

ResultsDialog::ResultsDialog(QWidget *parent, VoteHistory* vote_results, QuestVoteHistory* quest_results,
                             const std::vector<Subscriber*>& player_subscribers ) :
    QDialog(parent),
    ui(new Ui::ResultsDialog),
    player_subscribers_ref(player_subscribers)
{
    ui->setupUi(this);

    std::string result_text;

    if( vote_results != NULL ) {
        result_text = getVoteString( *vote_results, player_subscribers );
    } else {
        result_text = getQuestString( *quest_results, player_subscribers );
    }


    ui->resultsLabel->setText( QString( result_text.c_str() ) );

    connect( ui->acceptButton, SIGNAL( clicked( ) ), this, SLOT( close( ) ) );
}

std::string ResultsDialog::getVoteString( VoteHistory vote_results,
                                          const::std::vector<Subscriber*>& player_subscribers ) {

    std::string result_text = "";
    result_text += "The vote has ";

    if( vote_results.getVotePassed( ) ) {
        result_text += "passed";
    } else {
        result_text += "failed";
    }

    for ( unsigned int i = 0; i < vote_results.getPlayerVotes( ).size( ); i++ ) {
        if ( vote_results.getPlayerVotes( )[i] == avalon::YES) {
            result_text += "\n" + getPlayerName( i ) + " has voted to accept the team";
        } else if ( vote_results.getPlayerVotes( )[i] == avalon::NO) {
            result_text += "\n" + getPlayerName( i ) + " has voted to reject the team";
        } else if ( vote_results.getPlayerVotes( )[i] == avalon::HIDDEN) {
            // Handle hidden votes here if need be
        } else {
            result_text += "\n" + getPlayerName( i ) + " has apparently not voted, this shouldn't be possible";
        }
    }

    return result_text;
}

std::string ResultsDialog::getQuestString( QuestVoteHistory quest_results,
                                           const::std::vector<Subscriber*>& player_subscribers ) {
    std::string result_text = "Quest ";

    if( quest_results.getVotePassed( ) ) {
        result_text += "succeeded!";
    } else {
        result_text += "failed!";
    }

    std::vector<avalon::player_vote_t> votes = quest_results.getPlayerVotes( );

    int nPass = 0;
    int nFail = 0;
    for( int i = 0; i < votes.size( ); i++ ) {
        if( votes[i] == avalon::YES )
            nPass++;
        else if( votes[i] == avalon::NO )
            nFail++;
    }
    result_text += "\n " + std::to_string( nPass ) + " passes, " + std::to_string( nFail ) + " fails";

    return result_text;
}

ResultsDialog::~ResultsDialog()
{
    delete ui;
}

std::string ResultsDialog::getPlayerName( unsigned int player_id ) {
    return (player_subscribers_ref[ player_id ])->getData< Player >()->getName();
}
