#ifndef RESULTSDIALOG_H
#define RESULTSDIALOG_H

#include "voteHistory.hpp"
#include "questVoteHistory.hpp"
#include "player.hpp"
#include "subscriber.hpp"

#include <QDialog>

namespace Ui {
class ResultsDialog;
}

class ResultsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ResultsDialog(QWidget *parent, VoteHistory* vote_results, QuestVoteHistory* quest_results,
    	const std::vector<Subscriber*>& player_subscribers );
    ~ResultsDialog();

    std::string getVoteString( VoteHistory vote_results, const std::vector<Subscriber*>& player_subscribers );

    std::string getQuestString( QuestVoteHistory vote_results, const std::vector<Subscriber*>& player_subscribers );


private:
	const std::vector<Subscriber*>& player_subscribers_ref;
    Ui::ResultsDialog *ui;
    
    /**
     * Gets the name of the player at the given ID
     * @param player_id THe id of the player
     * @return The player's name
     */
    std::string getPlayerName( unsigned int player_id );
};

#endif // RESULTSDIALOG_H
