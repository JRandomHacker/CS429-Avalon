#ifndef OPTIONSWINDOW_H
#define OPTIONSWINDOW_H

#include "joinserverwindow.hpp"
#include "createserverwindow.hpp"
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
     * @return None
     */
    void on_buttonJoinMenu_clicked( );

    /**
     *  Closes the current window
     * @return None
     */
    void on_buttonQuit_clicked( );

    /**
     *  Opens create server window on click, closes the current window
     * @return None
     */
    void on_buttonCreateMenu_clicked( );

private:
    /**
     *  The field for the options window (its self).
     */
    Ui::OptionsWindow *ui;
    
    /**
     * Field for the window the join server button will lead to
     */
    JoinServerWindow* joinWindow;
    
    /**
     * Field for the button that the create server button will lead to
     */
    CreateServerWindow* createWindow;
};

#endif // OPTIONSWINDOW_H
