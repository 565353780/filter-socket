#ifndef DEEPLEARNINGSOLVER_H
#define DEEPLEARNINGSOLVER_H
#include "../Connection/Connection.h"

#ifdef WIN32
//#include <inference_engine.hpp>
//#include <samples/ocv_common.hpp>
//#include <ext_list.hpp>
#endif

#include "Darknet_Detector/Darknet_Detector.h"

#include <ctime>
#include <iomanip>
#include <QDebug>
#include <QDir>
#include <QVector>
#include <QString>
#include <QPainter>

#include "DataRebuild.h"
#include "LapNet_Train.h"
#include "received_files_manager.h"
#include "VideoCap.h"
#include "MaskDatasetController.h"

namespace FilterSocket {

class DeeplearningSolver
{
public:
    enum ErrorType
    {
        NoError = 0,
        Error = 1
    };

    DeeplearningSolver();

    void init(); // 初始化网络

    void init_model(QString conda_env_name);

    void close_server();

    int lapnet_process(Connection::ConnectedData &data);

    int darknet_process(Connection::ConnectedData &data);
    int darknet_test(Connection::ConnectedData &data);

    int lapnet_train_process(Connection::ConnectedData &data);
    int lapnet_stoptrain_process(Connection::ConnectedData &data);
    int lapnet_file_process(Connection::ConnectedData &data);
    int lapnet_getresult_process(Connection::ConnectedData &data);

    int darknet_createdata_process(Connection::ConnectedData &data);
    int darknet_enhancement_process(Connection::ConnectedData &data);
    int darknet_datachange_process(Connection::ConnectedData &data);
    int darknet_train_process(Connection::ConnectedData &data);
    int darknet_stoptrain_process(Connection::ConnectedData &data);
    int darknet_file_process(Connection::ConnectedData &data);
    int darknet_getresult_process(Connection::ConnectedData &data);
    int darknet_video_process(Connection::ConnectedData &data);
    int darknet_stopvideo_process(Connection::ConnectedData &data);

    int update_received_file_message_process(Connection::ConnectedData &data);

    int init_model_process(Connection::ConnectedData &data);

    Darknet_Detector darknet_detector;

    DataRebuild *data_rebuild_solver;
    LapNet_Train *lapnet_train_solver;

    Received_Files_Manager received_files_namager;
    VideoCap video_cap_manager;

    QImage camera_img;
    bool video_cap_start;
};


class DeeplearningFilter : public FilterInterface
{
public:
    DeeplearningFilter(std::shared_ptr<DeeplearningSolver> solver);

    int process(Connection::ConnectedData &data) override;

private:
    std::shared_ptr<DeeplearningSolver> solver_;
};
}
#endif // DEEPLEARNINGSOLVER_H
