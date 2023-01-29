#include "connectwindow.h"
#include <QApplication>

connectionManager g_connectionManager;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    ConnectWindow w;
    w.show();
    return a.exec();
}

//TODO:
//UPNP
