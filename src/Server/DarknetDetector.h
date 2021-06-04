#ifndef DARKNETDETECTOR_H
#define DARKNETDETECTOR_H

#include <QObject>
#include <QMainWindow>
#include <QWidget>
#include <QImage>
#include <QFileDialog>
#include <QTimer>
#include <QDebug>

#include <iostream>
#include <fstream>

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#ifdef WIN32
#include <yolo_v2_class.hpp>
#endif

#ifdef Linux
//extern "C"
//{
#include "../../include/darknet.h"
//}
#include <vector>
#include <string.h>
#endif

#include "../Connection/Connection.h"

using namespace FilterSocket;

enum NetworkType
{
    Yolov3Region = 0,
    Yolov3Component = 1,
    ResNetClassification = 2
};

#ifdef WIN32
class DarknetDetector : public QObject
{
    Q_OBJECT

public:
    explicit DarknetDetector(QObject *parent = nullptr);
    ~DarknetDetector();

public:
    void slot_LoadImage(QString image_path);
    void slot_LoadVideo(QString video_path);

    QImage cvMat2QImage(const cv::Mat& mat);

    cv::Mat QImage2cvMat(QImage image);

    void draw_boxes(cv::Mat mat_img, std::vector<bbox_t> result_vec, std::vector<std::string> obj_names,
        int current_det_fps = -1, int current_cap_fps = -1);

    std::vector<std::string> objects_names_from_file(std::string const filename);

    std::vector<bbox_t> getRegionResult(cv::Mat image);

    std::vector<bbox_t> getRegionResult(QImage image);

    std::vector<bbox_t> getComponentResult(cv::Mat image);

    std::vector<bbox_t> getComponentResult(QImage image);

    std::vector<bbox_t> getDarknetResult(cv::Mat mat);

    std::vector<bbox_t> getDarknetResult(QImage image);

private:
    void init_model(NetworkType network_type, QString names_file, QString cfg_file, QString weights_file);

    bool darknet_process(Connection::ConnectedData &data);

private:
    bool region_detector_loaded_;
    bool component_detector_loaded_;

    cv::Mat mat_;
    cv::VideoCapture capture_;

    Detector* region_detector_;
    std::vector<std::string> region_obj_names_;

    Detector* component_detector_;
    std::vector<std::string> component_obj_names_;

public slots:
    void slot_detect(Connection::ConnectedData& data);

signals:
    void signal_detect_finished();
};
#endif

#ifdef Linux
class Detector : public QObject
{
    Q_OBJECT

public:
    explicit Detector(const std::string &yolov3_cfg, const std::string &yolov3_weights, const std::string &coco_cfg, QObject *parent = nullptr);

    ~Detector();

public:
    std::vector<std::pair<char *, std::vector<float>>> detect(image im, float thresh=0.5, float hier_thresh=0.5, float nms=0.45);

    std::vector<std::pair<char *, std::vector<float>>> detect(char *img, float thresh=0.5, float hier_thresh=0.5, float nms=0.45);

    std::vector<std::pair<char *, std::vector<float>>> detect(float *img, int w, int h, int c, float thresh=0.5, float hier_thresh=0.5, float nms=0.45);

    network *net;
    metadata meta;
};

class DarknetDetector : public QObject
{
    Q_OBJECT

public:
    explicit DarknetDetector(QObject *parent = nullptr);

    ~DarknetDetector();

    std::vector<std::pair<char *, std::vector<float>>> getRegionResult(float *img, int w, int h, int c, float thresh=0.5, float hier_thresh=0.5, float nms=0.45);

    std::vector<std::pair<char *, std::vector<float>>> getComponentResult(float *img, int w, int h, int c, float thresh=0.5, float hier_thresh=0.5, float nms=0.45);

    std::vector<std::pair<char *, std::vector<float>>> getDarknetResult(image img, float thresh=0.5, float hier_thresh=0.5, float nms=0.45);

    std::vector<std::pair<char *, std::vector<float>>> getDarknetResult(char *img, float thresh=0.5, float hier_thresh=0.5, float nms=0.45);

    std::vector<std::pair<char *, std::vector<float>>> getDarknetResult(float *img, int w, int h, int c, float thresh=0.5, float hier_thresh=0.5, float nms=0.45);

private:
    void init_model(NetworkType network_type, QString coco_file, QString cfg_file, QString weights_file);

private:
    bool region_detector_loaded_;
    bool component_detector_loaded_;

    Detector* region_detector_;

    Detector* component_detector_;
};
#endif

#endif // DARKNETDETECTOR_H
