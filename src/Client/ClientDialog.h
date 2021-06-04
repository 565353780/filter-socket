#ifndef CLIENTDIALOG_H
#define CLIENTDIALOG_H

#include <QDialog>
#include <QFile>
#include <QDateTime>
#include <QFileDialog>
#include <QPixmap>
#include <QValidator>
#include <QHostAddress>
#include <QAbstractSocket>
#include <QImageReader>
#include <QTextCodec>
#include <QString>
#include <QJsonObject>
#include <QJsonDocument>
#include <QApplication>
#include <QPainter>
#include <QBitmap>
#include <QDir>

#include <ctime>
#include <iostream>

#include "ui_clientdialog.h"
#include "../Connection/Connection.h"
#include "Config/Setting/PolygonSetting.h"
#include "Config/Setting/PolygonListSetting.h"
#include "Config/Setting/NumSetting.h"
#include "Config/Config.h"
#include "Util/LogTool.h"
#include "WIdgets/ConfigWidget.h"

#ifdef WIN32
#include <windows.h>
#endif

#include <stdio.h>

#include "MaskWidget.h"
#include "CameraMaskWidget.h"
#include "MaskPolyWidget.h"
#include "MaskDatasetControlWidget.h"
#include "DataManagerWidget.h"
#include "ImageFile.h"

namespace Ui {
class ClientDialog;
}

class ClientDialog : public QDialog
{
    Q_OBJECT

public:  

    explicit ClientDialog(QWidget *parent = 0);
    ~ClientDialog();

private slots:
    void connect_to_server();

    void on_OpenImgButton_clicked();

    void on_ConnectServerBtn_clicked();

    void on_socket_connected();
    void on_socket_disconnected();
    void on_socket_error(QAbstractSocket::SocketError error);
    void on_DisconnectServerBtn_clicked();

    void on_processBtn_lapnet_clicked();
    void on_processBtn_darknet_clicked();
    void on_processBtn_poly_yolo_clicked();

    void on_SendFile();
    void on_SendFolderButton_clicked();

    void on_Btn_CreateData_clicked();
    void on_Btn_Enhancement_clicked();
    void on_Btn_DataChange_clicked();

    void on_Btn_Train_clicked();
    void on_Btn_StopTrain_clicked();
    void on_Btn_Valid_clicked();
    void on_Btn_GetResult_clicked();
    void on_Btn_SetResult_clicked();

    void on_SendTestFile();
    void on_Btn_DarknetTest_clicked();

    void on_Btn_GetReceivedMessage_clicked();
    void on_Btn_ClearAll_clicked();

    void on_Btn_InitModel_clicked();

    void on_Frame_send();
    void on_FrameBBox_get();
    void on_Video_send();
    void on_VideoBBox_get();

    void on_Btn_GetCameraFrame_clicked();
    void on_Btn_InitCamera_clicked();

    void on_Btn_CameraCap_clicked();
    void on_Btn_StopCameraCap_clicked();

    void on_Btn_SelectImageDir_clicked();
    void on_Btn_SelectOutputDir_clicked();

    void on_checkBox_ShowMask_stateChanged(int state);

    void on_checkBox_SaveCap_stateChanged(int state);

    void on_checkBox_ChoosePointPair_stateChanged(int state);

    void on_cBox_SolveMode_currentIndexChanged(int index);

    void on_cBox_folder_type_currentIndexChanged(int index);

    void on_process_finished();

public slots:
    void updateFileProgress(qint64);

private:
    Ui::ClientDialog *ui;
    FilterSocket::Connection *socket_;

    FilterSocket::Connection::ConnectedData data_;

    clock_t clock_start_{0};

    QImage input_img_copy_;

    QImage camera_img_copy_;
    bool video_mode_;
    bool save_camera_cap_;

    int bytesWritten{0};

    QString send_file_srcDirPath;
    QFileInfoList file_list;
    int current_send_file_index;

    QString current_img_name;

    QStringList yolov3_label_list;

    MaskConfig::ConfigWidget* config_widget;
    MaskWidget* mask_widget;
    CameraMaskWidget* camera_mask_widget;
    MaskPolyWidget* mask_poly_widget;
    MaskDatasetControlWidget* mask_dataset_control_widget;

    DataManagerWidget* data_manager_widget_;
};

#endif // CLIENTDIALOG_H
