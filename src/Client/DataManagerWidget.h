#ifndef DATAMANAGERWIDGET_H
#define DATAMANAGERWIDGET_H

#include <QWidget>
#include <QStandardItemModel>
#include <QFileDialog>
#include <QQueue>
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

#include "DataManager.h"
#include "DataLoader.h"

#pragma execution_character_set("utf-8")

namespace Ui {
class DataManagerWidget;
}

class DataManagerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DataManagerWidget(QWidget *parent = nullptr);
    ~DataManagerWidget();

    void init(QString db_path);

    void initSourceImageModel();
    void initMaskModel();
    void initPredictModel();

    bool insertFromMaskJsonObj(DataManager::NetworkType network_type, QString ImageID, QJsonObject mask_json_Obj);

    QJsonObject toMaskJsonObj(DataManager::NetworkType network_type, QString ImageID);

    bool insertFromMaskJsonFile(DataManager::NetworkType network_type, QString ImageID, QString mask_json_file_path);

    bool toMaskJsonFile(DataManager::NetworkType network_type, QString ImageID, QString mask_json_file_path);

private slots:
    void on_Btn_Init_Param_clicked();
    void on_Btn_Create_Work_Station_clicked();
    void on_Btn_Recycle_Mask_clicked();
    void on_Btn_Create_Train_Dataset_clicked();
    void on_Btn_Create_Valid_Dataset_clicked();
    void on_Btn_Recycle_Valid_clicked();

    void on_cBox_MaskMode_currentIndexChanged(int index);

    void on_Btn_OpenDir_clicked();

    void on_Btn_QueryMaskAndPredict_clicked();

    void on_Btn_InsertFolder_clicked();

    void on_checkBox_ConnectMask_stateChanged();

    void slot_ShowMsg(QString msg);

    void slot_ChangeProgressMaxNum(int max_progress_num);

    void slot_ChangeProgressNum(int progress_num);

signals:
    void signal_setLoadFileNumForEachFolder(int);

    void signal_LoadDir(QString, QStringList);

    void signal_CreateWorkStation();

    void signal_RecycleMask();

    void signal_CreateTrainDataset();

    void signal_CreateValidDataset();

    void signal_RecycleValid();

    void signal_InsertFolder(QString);

private:
    Ui::DataManagerWidget *ui;

    QDir dir_;

    DataManager* data_manager_;
    QThread* data_manager_thread_;
    bool data_manager_moved_;

    DataLoader* data_loader_;
    QThread* data_loader_thread_;

    QString current_open_dir_path_;
};

#endif // DATAMANAGERWIDGET_H
