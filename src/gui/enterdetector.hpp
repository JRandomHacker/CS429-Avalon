#ifndef ENTERDETECTOR_H
#define ENTERDETECTOR_H

#include <QObject>
#include <QPushButton>

class EnterDetector : public QObject
{
    Q_OBJECT
public:
    explicit EnterDetector(QObject *parent = 0, QPushButton* enterButton = 0);
    ~EnterDetector();

protected:
    bool eventFilter( QObject * obj, QEvent * event);

private:
    QPushButton* enterButton;

};

#endif // ENTERDETECTOR_H
