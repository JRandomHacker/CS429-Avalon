#include "resultsdialog.hpp"
#include "ui_resultsdialog.h"

ResultsDialog::ResultsDialog(QWidget *parent, const VoteHistory& vote_results, const std::vector<Subscriber*>& player_subscribers ) :
    QDialog(parent),
    ui(new Ui::ResultsDialog),
    player_subscribers_ref(player_subscribers)
{
    ui->setupUi(this);

    std::string result_text;

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

    ui->resultsLabel->setText( QString( result_text.c_str() ) );

    connect( ui->acceptButton, SIGNAL( clicked( ) ), this, SLOT( close( ) ) );
}

ResultsDialog::~ResultsDialog()
{
    delete ui;
}

std::string ResultsDialog::getPlayerName( unsigned int player_id ) {
    return (player_subscribers_ref[ player_id ])->getData< Player >()->getName();
}
