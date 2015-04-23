#ifndef ENDGAMEWINDOW_H
#define ENDGAMEWINDOW_H

#include <QDialog>
#include "player.hpp"

namespace Ui {
class EndGameWindow;
}

class EndGameWindow : public QDialog
{
    Q_OBJECT

public:
    EndGameWindow(QWidget *parent, std::vector<Player*> allPlayers, std::string resultString);
    ~EndGameWindow();

private:
    Ui::EndGameWindow *ui;
};

#endif // ENDGAMEWINDOW_H
