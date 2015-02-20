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

private:
    Ui::OptionsWindow *ui;
    
};

#endif // OPTIONSWINDOW_H
