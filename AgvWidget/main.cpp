#include "agvmainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
QApplication a(argc, argv);
AgvMainWindow w;
w.show();

return a.exec();
}
