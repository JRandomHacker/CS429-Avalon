#ifndef JOINSERVERWINDOW_H
#define JOINSERVERWINDOW_H

#include <QDialog>

namespace Ui {
class JoinServerWindow;
}

class JoinServerWindow : public QDialog
{
    Q_OBJECT

public:
    explicit JoinServerWindow(QWidget *parent = 0);
    ~JoinServerWindow();

private:
    Ui::JoinServerWindow *ui;
};

#endif // JOINSERVERWINDOW_H
