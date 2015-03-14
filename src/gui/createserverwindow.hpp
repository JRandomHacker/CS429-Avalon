#ifndef CREATESERVERWINDOW_H
#define CREATESERVERWINDOW_H

#include <QDialog>

#ifdef _WIN32
    #include <windows.h>
#endif

#define MAX_ARGS 8

namespace Ui {
class CreateServerWindow;
}

/**
 *  Create Server window class. This is the window where the game is created.
 *  Displays the options and settings to create a new game.
 *	Spawns listen server in a new process.
 */
class CreateServerWindow : public QDialog {
    Q_OBJECT

public:
	/**
 	 *  Creates a new server window.
 	 *	@param	parent	pointer to parent window
 	 */
    explicit CreateServerWindow(QWidget *parent = 0);

    ~CreateServerWindow( );

private slots:
	/*
	 *  calls create server on click
	 *	displays respective error message if create server fails.
	 */
    void on_buttonCreateServer_clicked( );
    
    /*
     *  Closes this window and opens the main menu
     */
    void on_buttonCancel_clicked( );

private:
	/**
 	 *  UI field for create server window
 	 */
    Ui::CreateServerWindow *ui;
    
    #ifdef _WIN32
        /**
         *  Server handle
         */
        HANDLE serverH;
    #else
        /**
         *  Server pid
         */
        int serverH;
    #endif
    
    /*
 	 *  Creates the server in a new process.
 	 */
    int createServer( );

    /*
 	 *	Connects to an existing server
 	 *	creates a game window, closes the current window.
 	 */
    void connectToServer( );
    
};

#endif // CREATESERVERWINDOW_H
