#ifndef QUESTHISTORYWINDOW_H
#define QUESTHISTORYWINDOW_H

#include <QMainWindow>

namespace Ui {
class questHistoryWindow;
}

class questHistoryWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit questHistoryWindow(QWidget *parent = 0);
    ~questHistoryWindow();

private:
    Ui::questHistoryWindow *ui;
};

#endif // QUESTHISTORYWINDOW_H
