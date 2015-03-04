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

public slots:
    void createServer();
    void joinServerSlot();

private:
    Ui::OptionsWindow *ui;
    void joinServer( std::string host, int port );
};

#endif // OPTIONSWINDOW_H
