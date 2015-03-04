#ifndef CREATESERVERWINDOW_H
#define CREATESERVERWINDOW_H

#include <QDialog>

namespace Ui {
class CreateServerWindow;
}

class CreateServerWindow : public QDialog
{
    Q_OBJECT

public:
    explicit CreateServerWindow(QWidget *parent = 0);
    ~CreateServerWindow();
    void createServer();

private slots:
    void on_buttonCreateServer_clicked();

private:
    Ui::CreateServerWindow *ui;
};

#endif // CREATESERVERWINDOW_H
