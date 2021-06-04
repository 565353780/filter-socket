#ifndef Darknet_Detector_H
#define Darknet_Detector_H

#include <iostream>
#include <fstream>

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "darknet.h"
#include "image.h"

#include <vector>
#include <string.h>

using namespace std;
using namespace cv;

class Darknet_Detector
{
public:
    Darknet_Detector();
    ~Darknet_Detector();

public:
    void init_model(string cfg_file, string weights_file);

    bool detect_image(Mat image);

    vector<vector<double>> get_result()
    {
        return result_;
    }

    vector<vector<int>> get_result_trans()
    {
        return result_trans_;
    }

private:
    image ipl_to_image(IplImage* src);

    bool detect(Mat input);

private:
    bool detector_loaded_;
    network *net_;

    float thresh_;
    float hier_thresh_;
    float nms_;

    Mat input_;

    vector<vector<double>> result_;
    vector<vector<int>> result_trans_;
};

#endif // Darknet_Detector_H
