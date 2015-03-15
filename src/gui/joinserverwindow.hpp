#ifndef JOINSERVERWINDOW_H
#define JOINSERVERWINDOW_H

#include <QDialog>

#include "model.hpp"
#include "clientController.hpp"

namespace Ui {
class JoinServerWindow;
}

class JoinServerWindow : public QDialog {
    Q_OBJECT

public:
	/**
     *  Creates a join server window.
     *  @param   parent		Pointer to parent widget.
     */
    explicit JoinServerWindow( QWidget *parent = 0 );

    /**
     *  Destroys the join server window.
     */
    ~JoinServerWindow( );

private slots:

	/**
	 *	Creates a client model and controller.
     *  Joins a game server on click. 
     */
    void on_buttonJoinServer_clicked( );
    
    /**
     *  Closes this window and opens the main menu
     */
    void on_buttonCancel_clicked();

private:
	/**
     *  Join server ui field.
     */
    Ui::JoinServerWindow *ui;

    /**
     * The Model we will watch later
     */
    Model* model;

    /**
     * The ClientController to modify the model for us
     */
    ClientController* controller;

};

#endif // JOINSERVERWINDOW_H
