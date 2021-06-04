#ifndef RECEIVED_FILES_MANAGER_H
#define RECEIVED_FILES_MANAGER_H
#include <QString>
#include <QDir>
#include <QDebug>
#include <QStringList>
#include <QFileInfoList>
#include "../Connection/Connection.h"

class Received_Files_Manager
{
public:
    Received_Files_Manager();
    ~Received_Files_Manager();

    bool check_files_changed();

    bool update_files_message();

private:
    QStringList files_list;
    QStringList files_list_copy;
};

#endif // RECEIVED_FILES_MANAGER_H
