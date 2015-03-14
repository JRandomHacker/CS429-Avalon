#ifndef JOINSERVERWINDOW_H
#define JOINSERVERWINDOW_H

#include <QDialog>

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

private:
	/**
     *  Join server ui field.
     */
    Ui::JoinServerWindow *ui;
};

#endif // JOINSERVERWINDOW_H
