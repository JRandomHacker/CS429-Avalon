#include "enterdetector.hpp"
#include <QKeyEvent>

EnterDetector::EnterDetector(QObject *parent, QPushButton* enterButton ) : QObject(parent) {
    this->enterButton = enterButton;
}

EnterDetector::~EnterDetector() { }

bool EnterDetector::eventFilter( QObject * obj, QEvent * event) {
    if( event->type( ) == QEvent::KeyPress ) {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>( event );
        if( keyEvent->key( ) == Qt::Key_Return ) {
            enterButton->click( );
            return true;
        }
    }

    return QObject::eventFilter( obj, event );
}
