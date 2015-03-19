#ifndef RESULTSDIALOG_H
#define RESULTSDIALOG_H

#include "voteHistory.hpp"
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
    explicit ResultsDialog(QWidget *parent, const VoteHistory& vote_results,
    	const std::vector<Subscriber*>& player_subscribers );
    ~ResultsDialog();


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
