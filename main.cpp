#include "connectwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    connectionHandler h;
    ConnectWindow w(h);
    w.show();
    return a.exec();
}
