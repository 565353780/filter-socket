#ifndef MASKDATASETCONTROLWIDGET_H
#define MASKDATASETCONTROLWIDGET_H

#include <QWidget>
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

namespace Ui {
class MaskDatasetControlWidget;
}

class MaskDatasetControlWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MaskDatasetControlWidget(QWidget *parent = nullptr);
    ~MaskDatasetControlWidget();

private slots:
    void on_Btn_Source_Dataset_Path_clicked();
    void on_Btn_Target_Dataset_Path_clicked();
    void on_Btn_Init_Param_clicked();
    void on_Btn_Create_Work_Station_clicked();
    void on_Btn_Recycle_Mask_clicked();
    void on_Btn_Create_Train_Dataset_clicked();
    void on_Btn_Create_Valid_Dataset_clicked();
    void on_Btn_Recycle_Valid_clicked();

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
    Ui::MaskDatasetControlWidget *ui;

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

#endif // MASKDATASETCONTROLWIDGET_H
