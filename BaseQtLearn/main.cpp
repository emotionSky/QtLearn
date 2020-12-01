#include "baseqtwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    BaseQtWindow w;
    w.show();
    return a.exec();
}
