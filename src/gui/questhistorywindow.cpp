#include "questhistorywindow.hpp"
#include "ui_questhistorywindow.h"

questHistoryWindow::questHistoryWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::questHistoryWindow)
{
    ui->setupUi(this);
}

questHistoryWindow::~questHistoryWindow()
{
    delete ui;
}
