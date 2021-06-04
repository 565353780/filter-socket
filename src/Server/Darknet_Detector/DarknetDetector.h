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

#include "Darknet_Detector/yolo_v2_class.hpp"
#include "darknet.h"

#include <vector>
#include <string.h>

using namespace std;
using namespace cv;

class DarknetDetector : public QObject
{
    Q_OBJECT

public:
    explicit DarknetDetector(QObject *parent = nullptr);
    ~DarknetDetector();

public:
    void draw_boxes(Mat mat_img, vector<bbox_t> result_vec, vector<string> obj_names,
        int current_det_fps = -1, int current_cap_fps = -1);

    vector<string> objects_names_from_file(string const filename);

    bool detect_image(Mat image);
    bool detect_image(QImage image);

private:
    void init_model(QString names_file, QString cfg_file, QString weights_file);

    QImage cvMat2QImage(const Mat& mat);

    Mat QImage2cvMat(QImage image);

    bool detect(Mat image);

private:
    bool detector_loaded_;

    Mat mat_;
    VideoCapture capture_;

    Detector *darknet_detector_;
    vector<string> obj_names_;

    vector<bbox_t> result_vec_;
};

#endif // DARKNETDETECTOR_H
