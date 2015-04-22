#include "endgamewindow.h"
#include "ui_endgamewindow.h"

endGameWindow::endGameWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::endGameWindow)
{
    ui->setupUi(this);
}

endGameWindow::~endGameWindow()
{
    delete ui;
}
