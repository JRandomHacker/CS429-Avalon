#ifndef OPTIONSWINDOW_H
#define OPTIONSWINDOW_H

#include <QMainWindow>

namespace Ui {
class OptionsWindow;
}

/**
 *  Main window for the application, allows for join/create game, and quit application.
 */
class OptionsWindow : public QMainWindow {
    Q_OBJECT

public:
    /**
     *  Constructor that creates new options window.
     *  @param   parent     Pointer to parent widget.
     */
    explicit OptionsWindow( QWidget *parent = 0 );

    /**
     *  Destroys the options window.
     */
    ~OptionsWindow( );

private slots:
    /**
     *  Opens joins server window on click, closes the current window
     */
    void on_buttonJoinMenu_clicked( );

    /**
     *  Closes the current window
     */
    void on_buttonQuit_clicked( );

    /**
     *  Opens create server window on click, closes the current window
     */
    void on_buttonCreateMenu_clicked( );

private:
    /**
     *  Options window field.
     */
    Ui::OptionsWindow *ui;
};

#endif // OPTIONSWINDOW_H
