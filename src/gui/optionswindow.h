#ifndef OPTIONSWINDOW_H
#define OPTIONSWINDOW_H

#include <QMainWindow>

namespace Ui {
class OptionsWindow;
}

class OptionsWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit OptionsWindow(QWidget *parent = 0);
    ~OptionsWindow();

private slots:
    void on_buttonJoinMenu_clicked();

    void on_buttonQuit_clicked();

    void on_buttonCreateMenu_clicked();

private:
    Ui::OptionsWindow *ui;
    void createServer();
};

#endif // OPTIONSWINDOW_H
