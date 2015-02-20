#include "optionswindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    OptionsWindow w;
    w.show();

    return a.exec();
}
