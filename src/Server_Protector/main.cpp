#include <iostream>
#include <QDir>
#include <QFile>
#include <QByteArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QDebug>

int main()
{
    QDir dir_;

    bool server_already_start = true;

    QFile file_json("../Server_DataBase/message_set/server.json");

    if(!dir_.exists("../Server_DataBase/message_set/server.json"))
    {
        if(!file_json.open(QFile::WriteOnly))
        {
            qDebug() << "main : server.json write failed.";
            return -1;
        }

        QJsonObject obj;
        obj["server_started"] = false;
        obj["load_net"] = false;
        obj["thresh"] = 0.5;

        QJsonDocument doc;
        doc.setObject(obj);

        QByteArray array;
        array.push_back(doc.toJson());

        file_json.write(array.toBase64());

        file_json.close();
    }
    else
    {
        if(!file_json.open(QFile::ReadOnly))
        {
            qDebug() << "main : server.json read failed.";
            return -1;
        }

        QByteArray array = QByteArray::fromBase64(file_json.readAll());

        QJsonDocument doc = QJsonDocument::fromJson(array);

        QJsonObject obj = doc.object();

        if(!obj["server_started"].toBool())
        {
            server_already_start = false;
        }

        file_json.close();
    }

    if(!server_already_start)
    {
#ifdef WIN32
        system("start Server.exe&&exit");
#endif
#ifdef Linux
        system("RUN_AFTER_BASHRC=\"./Server&&exit\" gnome-terminal");
#endif
    }

    QFile file("../Server_DataBase/message_set/server_stopped.txt");

    while(true)
    {
        if(!file.exists())
        {
            continue;
        }

        while(!file.remove())
        {
            continue;
        }

#ifdef WIN32
        system("start Server.exe&&exit");
#endif
#ifdef Linux
        system("RUN_AFTER_BASHRC=\"./Server&&exit\" gnome-terminal");
#endif
    }

    return 1;
}
