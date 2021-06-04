#include <QTcpSocket>
#include <QCoreApplication>
#include <QHostAddress>
#include <QImage>
#include <QPixmap>
#include <QBuffer>
#include <ctime>
#include "../Connection/Connection.h"
#include "ClientDialog.h"
#include <QApplication>
#include <QTranslator>

int main(int argc, char **argv)
{
    QApplication a(argc,argv);
    QDir::setCurrent(a.applicationDirPath());

    QTranslator translator;
    translator.load("tt_client.qm");
    a.installTranslator(&translator);

    QDir dir("../Client_DataBase/message_set");

    if(!dir.exists())
    {
        dir.mkpath(dir.absolutePath());
    }

//    FilterSocket::Connection socket;
//    socket.connectToHost("127.0.0.1",13141);

//    socket.connect(&socket,SIGNAL(readyRead()),&socket,SLOT(processData()));
//    QImage img;
//    img.load("client.png");
//    clock_t t0 = clock();


//    QByteArray array(img.width() * img.height() * 3,0);
//    for(int i=0; i < img.width(); i++)
//    {
//        for(int j=0; j < img.height(); j++)
//        {
//            QRgb rgb = img.pixel(i,j);
//            array[3* (j * img.width() + i) ] = char(qRed(rgb));
//            array[3* (j * img.width() + i) + 1 ] = char(qBlue(rgb));
//            array[3* (j * img.width() + i) + 2] = char(qGreen(rgb));

//        }
//    }

//    qDebug()<<clock()-t0<<"ms "<<array.size();
//    t0 = clock();

//    socket.write(FilterSocket::Connection::toHeader(img.width(),img.height(),3)+ array);
//    qDebug()<<clock() - t0<<"ms"<<" "<<1280*1024*3;

    ClientDialog w;
    w.show();
    return a.exec();

}
