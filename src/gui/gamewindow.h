#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QDialog>
#include "clientController.hpp"
#include "model.hpp"

namespace Ui {
class GameWindow;
}

class GameWindow : public QDialog
{
    Q_OBJECT

public:
    explicit GameWindow(QWidget *parent = 0, ClientController* controller = NULL, Model * model = NULL);
    ~GameWindow();

private:
    Ui::GameWindow *ui;
    ClientController * control;
    Model * model;
};

#endif // GAMEWINDOW_H
