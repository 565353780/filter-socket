#include <QCoreApplication>
#include "ImgFilterServer.h"
#include <QApplication>
#include  "ServerDialog.h"
#include <iostream>
#include <QDir>
#include <QTranslator>

int main(int argc, char **argv)
{
    QApplication a(argc,argv);

    QTranslator translator;
    translator.load("tt_server.qm");
    a.installTranslator(&translator);

    QDir dir("../Server_DataBase/message_set");

    if(!dir.exists())
    {
        dir.mkpath(dir.absolutePath());
    }

    int port = 13141;

    ServerDialog w(port);
    w.show();
    FilterSocket::ImgFilterServer *server = new FilterSocket::ImgFilterServer(port,&w);
    w.connect(server,SIGNAL(signalState(QString)),&w,SLOT(slotState(QString)));
    server->init();

    return a.exec();
}
