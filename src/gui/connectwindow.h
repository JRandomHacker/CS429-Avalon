#ifndef CONNECTWINDOW_H
#define CONNECTWINDOW_H

#include "globals.hpp"
#include <QDialog>
#include <string>

namespace Ui {
class ConnectWindow;
}

class ConnectWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ConnectWindow(QWidget *parent = 0, std::string ip = "localhost", int port = DEFAULT_PORT);
    ~ConnectWindow();

private slots:
    void on_dummyConnect_clicked();
    
private:
    Ui::ConnectWindow *ui;
};

#endif // CONNECTWINDOW_H
