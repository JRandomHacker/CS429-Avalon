#ifndef CREATESERVERWINDOW_H
#define CREATESERVERWINDOW_H

#include <QDialog>

#define MAX_ARGS 8

namespace Ui {
class CreateServerWindow;
}

class CreateServerWindow : public QDialog
{
    Q_OBJECT

public:
    explicit CreateServerWindow(QWidget *parent = 0);
    ~CreateServerWindow();

private slots:
    void on_buttonCreateServer_clicked();

private:
    Ui::CreateServerWindow *ui;
    int createServer();
    void connectToServer();
};

#endif // CREATESERVERWINDOW_H
