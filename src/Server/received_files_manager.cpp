#include "received_files_manager.h"

Received_Files_Manager::Received_Files_Manager()
{

}

Received_Files_Manager::~Received_Files_Manager()
{

}

bool Received_Files_Manager::check_files_changed()
{
    QDir dir("../Server_DataBase/message_set");

    if(!dir.exists())
    {
        dir.mkdir(dir.absolutePath());

        return true;
    }

    QDir files_root_dir("../Server_DataBase");

    QFileInfoList file_info_list = FilterSocket::Connection::GetFileList(files_root_dir.absolutePath());

    files_list.clear();

    for(int i = 0; i < file_info_list.size(); ++i)
    {
        if(!file_info_list[i].absoluteFilePath().contains("message_set"))
        {
            files_list.append(file_info_list[i].absoluteFilePath().split("Server_DataBase/")[1]);
        }
    }

    if(files_list.size() != files_list_copy.size())
    {
        return true;
    }

    else
    {
        for(int i = 0; i < files_list.size(); ++i)
        {
            if(files_list[i] != files_list_copy[i])
            {
                return true;
            }
        }
    }

    return false;
}

bool Received_Files_Manager::update_files_message()
{
    if(!check_files_changed())
    {
        return false;
    }

    QFile file("../Server_DataBase/message_set/received_files_list.txt");

    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "Failed to open received_files_list.txt to write !" << endl;

        return false;
    }

    file.resize(0);

    files_list_copy.clear();

    for(int i = 0; i < files_list.size(); ++i)
    {
        file.write((files_list[i] + "\n").toStdString().c_str());

        files_list_copy.append(files_list[i]);
    }

    file.close();

    return true;
}
