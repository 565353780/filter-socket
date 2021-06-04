#include "Example.h"

Example::Example()
{

}

Example::~Example()
{

}

void Example::run_example()
{
    Darknet_Detector darknet_detector;

    string region_cfg_file = "../data/waterdrop/yolov3.cfg";
    string region_weights_file = "../data/waterdrop/yolov3_train_waterDrop_2class.backup";

    darknet_detector.init_model(region_cfg_file, region_weights_file);

    //1 : image 2 : video
    int mode = 2;

    if(mode == 1)
    {
        Mat input = imread("../data/waterdrop/20201222164429.jpg");

        darknet_detector.detect_image(input);

        vector<vector<double>> result = darknet_detector.get_result();

        cout << result.size() << endl;

        for(int i = 0; i < result.size(); ++i)
        {
            vector<double> bbox = result[i];

            int left  = bbox[0] * input.cols;
            int top   = bbox[1] * input.rows;
            int right = bbox[2] * input.cols;
            int bot   = bbox[3] * input.rows;

            if(left < 0) left = 0;
            if(right > input.cols-1) right = input.cols-1;
            if(top < 0) top = 0;
            if(bot > input.rows-1) bot = input.rows-1;

            cout << i << " : " << left << "," << top << " ; " << right << "," << bot << endl;

            rectangle(input, Point(left, top), Point(right, bot), Scalar(0, 0, 255), 2);
        }

        imshow("result.jpg", input);
        waitKey(0);
    }
    else if(mode == 2)
    {
        Mat input;

        VideoCapture cap;

        cap.open("/home/chli/chLi/Project/ABACI/WaterDropDetect/data/slowest.avi");

        while(cap.isOpened())
        {
            cap >> input;

            darknet_detector.detect_image(input);

            vector<vector<double>> result = darknet_detector.get_result();

            cout << result.size() << endl;

            for(int i = 0; i < result.size(); ++i)
            {
                vector<double> bbox = result[i];

                int left  = bbox[0] * input.cols;
                int top   = bbox[1] * input.rows;
                int right = bbox[2] * input.cols;
                int bot   = bbox[3] * input.rows;

                if(left < 0) left = 0;
                if(right > input.cols-1) right = input.cols-1;
                if(top < 0) top = 0;
                if(bot > input.rows-1) bot = input.rows-1;

                cout << i << " : " << left << "," << top << " ; " << right << "," << bot << endl;

                rectangle(input, Point(left, top), Point(right, bot), Scalar(0, 0, 255), 2);
            }

            imshow("result.jpg", input);
            waitKey(1);
        }
    }
}
