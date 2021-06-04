#include "DataLoader.h"

DataLoader::DataLoader(QObject *parent)
    : QObject(parent)
{
    each_folder_select_image_num_ = -1;
    ignore_folder_name_list_.clear();
    ignore_folder_name_list_.append("MaskSpace");
    ignore_folder_name_list_.append("config");
    ignore_folder_name_list_.append("valid_dataset");
    ignore_folder_name_list_.append("train_dataset");

    find_files_num_ = 0;
}

DataLoader::~DataLoader()
{

}

void DataLoader::countFiles(QString dir_path)
{
    dir_.setPath(dir_path);

    QStringList file_name_list = dir_.entryList(QDir::Files);

    int current_opened_image_num = 0;

    for(QString subdir : dir_.entryList(QDir::AllDirs | QDir::NoDotAndDotDot))
    {
        if(!ignore_folder_name_list_.contains(subdir))
        {
            countFiles(dir_path + subdir + "/");
        }
    }

    for(QString file_name : file_name_list)
    {
        ++current_opened_image_num;

        if(each_folder_select_image_num_ > -1)
        {
            if(current_opened_image_num > each_folder_select_image_num_)
            {
                return;
            }
        }

        ++find_files_num_;
    }
}

void DataLoader::findFiles(QString dir_path)
{
    dir_.setPath(dir_path);

    QStringList file_name_list = dir_.entryList(QDir::Files);

    int current_opened_image_num = 0;

    for(QString subdir : dir_.entryList(QDir::AllDirs | QDir::NoDotAndDotDot))
    {
        if(!ignore_folder_name_list_.contains(subdir))
        {
            findFiles(dir_path + subdir + "/");
        }
    }

    for(QString file_name : file_name_list)
    {
        ++current_opened_image_num;

        if(each_folder_select_image_num_ > -1)
        {
            if(current_opened_image_num > each_folder_select_image_num_)
            {
                return;
            }
        }

        emit signal_AddOpenFilePath(dir_path + file_name);
    }
}

void DataLoader::countDir(QString dir_path, QStringList filters)
{
    find_files_num_ = 0;

    QString abs_dir_path = dir_path;

    if(!dir_.isAbsolutePath(abs_dir_path))
    {
        abs_dir_path = dir_.absoluteFilePath(abs_dir_path);
    }

    dir_.setNameFilters(filters);

    countFiles(abs_dir_path);

    emit signal_findFilesNum(find_files_num_);
}

void DataLoader::loadDir(QString dir_path, QStringList filters)
{
    QString abs_dir_path = dir_path;

    if(!dir_.isAbsolutePath(abs_dir_path))
    {
        abs_dir_path = dir_.absoluteFilePath(abs_dir_path);
    }

    dir_.setNameFilters(filters);

    findFiles(abs_dir_path);

    emit signal_LoadDir_finished();
}

void DataLoader::slot_LoadDir(QString dir_path, QStringList filters)
{
    countDir(dir_path, filters);

    loadDir(dir_path, filters);
}

void DataLoader::slot_setLoadFileNumForEachFolder(int load_file_num_for_each_folder)
{
    each_folder_select_image_num_ = load_file_num_for_each_folder;
}
