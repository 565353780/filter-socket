#ifndef MASKDATASETCONTROLLER_H
#define MASKDATASETCONTROLLER_H

#include <QString>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QFileInfoList>
#include <QFileDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#pragma execution_character_set("utf-8")
#define TEXT(str) std::string((const char *)(str).toLocal8Bit())

class MaskDatasetController
{
public:
    MaskDatasetController();
    ~MaskDatasetController();

private:
    void create_dataset_struct(QString path_to_root="");

    void get_file_list(QString path_to_root="");

    void get_absolute_file_path_list();

    void create_work_station();

    void copy_unmarked_dataset();

public:
    void init_param(QString root_path, QString target_path, int target_dataset_num, int workers_num);

    void create_mask_work_station();

    void recycle_mask_dataset();

    void create_train_dataset();

    void create_valid_dataset();

    void recycle_valid_dataset();

    QVector<QString> get_source_file_path(){return source_file_path_;}

private:
    QString root_path_;
    QString target_path_;
    int target_dataset_num_;
    int workers_num_;

    int source_dataset_num_;
    int unmarked_dataset_num_;

    QVector<QString> source_file_path_;
    QVector<QString> unmarked_file_path_;

    int dataset_num_for_each_worker_;
    int res_dataset_num_;
    QString work_path_;

    bool use_all_unmarked_dataset_;

    int debug_skip_num_;

    QString root_path_copy_;
    bool get_file_list_finished_;
};

#endif MASKDATASETCONTROLLER_H
