#ifndef ENDGAMEWINDOW_H
#define ENDGAMEWINDOW_H

#include <QDialog>

namespace Ui {
class endGameWindow;
}

class endGameWindow : public QDialog
{
    Q_OBJECT

public:
    explicit endGameWindow(QWidget *parent = 0);
    ~endGameWindow();

private:
    Ui::endGameWindow *ui;
};

#endif // ENDGAMEWINDOW_H
