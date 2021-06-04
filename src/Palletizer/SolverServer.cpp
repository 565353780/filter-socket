/****************************************************************************
**
** Copyright (C) 2016 Kurt Pattyn <pattyn.kurt@gmail.com>.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtWebSockets module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/
#include "SolverServer.h"
#include "QtWebSockets/QWebSocketServer"
#include "QtWebSockets/QWebSocket"
#include <QtCore/QDebug>
#include "Palletizer.h"
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
QT_USE_NAMESPACE

//! [constructor]
SolverServer::SolverServer(quint16 port, QObject *parent) :
    QObject(parent),
    m_pWebSocketServer(Q_NULLPTR)
{
    m_pWebSocketServer = new QWebSocketServer(QStringLiteral("Chat Server"),
                                              QWebSocketServer::NonSecureMode,
                                              this);
    if (m_pWebSocketServer->listen(QHostAddress::Any, port))
    {
        qDebug() << "Chat Server listening on port" << port;
        connect(m_pWebSocketServer, &QWebSocketServer::newConnection,
                this, &SolverServer::onNewConnection);
    }
}

SolverServer::~SolverServer()
{
    m_pWebSocketServer->close();
    qDeleteAll(m_clients.begin(), m_clients.end());
}
//! [constructor]

//! [onNewConnection]
void SolverServer::onNewConnection()
{
    QWebSocket *pSocket = m_pWebSocketServer->nextPendingConnection();

    connect(pSocket, &QWebSocket::textMessageReceived, this, &SolverServer::processMessage);
    connect(pSocket, &QWebSocket::disconnected, this, &SolverServer::socketDisconnected);

    m_clients << pSocket;
}
//! [onNewConnection]

//! [processMessage]
void SolverServer::processMessage(const QString &message)
{
    auto *pSender = qobject_cast<QWebSocket *>(sender());

    QJsonDocument doc;
    QJsonParseError error{};
    doc = QJsonDocument::fromJson(message.toUtf8(),&error);
    QJsonObject obj = doc.object();
    int flag = 0;
    Palletizer pz;
    if(obj.contains("platform"))
    {
        flag = 1;
        QJsonArray platform = obj.value("platform").toArray();
        if(platform.size() < 2) {
            flag = 0;
        }
        else {
            pz.init(platform.at(0).toInt(1000),platform.at(1).toInt(1000));
        }
    }
    if(flag == 1 && obj.contains("list"))
    {
       std::vector<QueryBox> qblist;
       QJsonObject list = obj.value("list").toObject();
       for(auto iter = list.begin(); iter != list.end(); iter++)
       {
            QJsonObject bbox = iter.value().toObject();
            int num = bbox.value("num").toInt(0);
            for(int i=0; i < num; i++)
            {
                int w = bbox.value("w").toInt(100);
                int h = bbox.value("h").toInt(100);
                int d = bbox.value("d").toInt(100);
                qblist.emplace_back(w,h,d);
            }
       }
       pz.loadQueryList(qblist);
       flag = 2;
    }
    if(flag == 2)
    {
        if(obj.contains("type"))
        {
            int type = obj.value("type").toInt(0);
            flag += type;
        }
    }
    if(flag == 2 )
    {
        qDebug()<<"Flag 2";
        pz.setOnlineNum(1);
        pz.setMaxTestNumAtOnce(1);
        pz.initOrder();
        pz.solveOnLine();
    }
    else if(flag == 3)
    {
        qDebug()<<"Flag 3";
        pz.setOnlineNum(0);
        pz.initOrder();
        pz.solveOffLine();
    }
    else if(flag == 4)
    {
        qDebug()<<"Flag 4";
    }
    else
    {
        qDebug()<<"Flag "<<flag;
        pz.setOnlineNum(3);
        pz.setMaxTestNumAtOnce(2);
        pz.solveOnLine();

    }

    pSender->sendTextMessage(pz.toJson());
}
//! [processMessage]

//! [socketDisconnected]
void SolverServer::socketDisconnected()
{
    auto *pClient = qobject_cast<QWebSocket *>(sender());
    if (pClient)
    {
        m_clients.removeAll(pClient);
        pClient->deleteLater();
    }
}
//! [socketDisconnected]
