#include "ImgFilterServer.h"
#include <QDebug>
#include <QJsonObject>
#include <QJsonDocument>
#include "../Connection/Connection.h"
#include "DeeplearningSolver.h"
namespace FilterSocket {

ImgFilterServer::ImgFilterServer(quint16 port, QObject *parent):QTcpServer(parent),solver_(new DeeplearningSolver())
{   
    qDebug()<<"Listen port:"<<port<<" "<< listen(QHostAddress::Any,port);
}

void ImgFilterServer::init()
{
    emit this->signalState(tr("Init deeplearning net..."));
    solver_->init();

    emit this->signalState(tr("Init deeplearning system done!"));
}

void ImgFilterServer::slotDisconnected()
{
    qDebug()<<"Disconnected";
}

void ImgFilterServer::incomingConnection(qintptr handle)
{
    Connection *connection = new Connection(this);


    connection->setSocketDescriptor(handle);

    emit this->signalState(tr("new Connection:")+QString::number(connection->localPort()));

    connection->setFilter(std::shared_ptr<FilterInterface>(new DeeplearningFilter(solver_)));

    connect(connection,SIGNAL(readyRead()),connection,SLOT(processData()));
    connect(connection,SIGNAL(disconnected()),this,SLOT(slotDisconnected()));
    connect(connection,SIGNAL(disconnected()),connection,SLOT(deleteLater()));
}



}
