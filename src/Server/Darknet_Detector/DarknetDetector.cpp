#include "DarknetDetector.h"

#define OPENCV
#define GPU

//#pragma comment(lib, "D:/chLi/Project/ABACI/FilterSocket/bin_win/darknet-win-gpu/yolo_cpp_dll.lib")//引入链接库

DarknetDetector::DarknetDetector(QObject *parent) : QObject (parent)
{
    detector_loaded_ = false;

    //Load Region Detector [每张图标注前两排立柱]
    QString region_names_file = "D:/chLi/Project/ABACI/FilterSocket/bin_win/darknet-win-gpu/yolov3_train_2c_region_2row_mask_1class/coco.names";
    QString region_cfg_file = "D:/chLi/Project/ABACI/FilterSocket/bin_win/darknet-win-gpu/yolov3_train_2c_region_2row_mask_1class/yolov3.cfg";
    QString region_weights_file = "D:/chLi/Project/ABACI/FilterSocket/bin_win/darknet-win-gpu/yolov3_train_2c_region_2row_mask_1class/yolov3_train_2c_region_2row_mask_1class.backup";

    init_model(region_names_file, region_cfg_file, region_weights_file);

    detector_loaded_ = true;

    qDebug() << "init detector finished.";
}

DarknetDetector::~DarknetDetector()
{

}

//void DarknetDetector::slot_LoadVideo(QString video_path)
//{
//    capture_.open(video_path.toStdString());
//    if (!capture_.isOpened())
//    {
//        printf("文件打开失败");
//    }
//    Mat frame;

//    while (true)
//    {
//        capture_ >> frame;
//        vector<bbox_t> result_vec = darknet_detector_->detect(frame);
//        draw_boxes(frame, result_vec, obj_names_);

//        namedWindow("test", CV_WINDOW_NORMAL);
//        imshow("test", frame);
//        waitKey(3);
//    }
//}

void DarknetDetector::draw_boxes(Mat mat_img, vector<bbox_t> result_vec, vector<string> obj_names,
    int current_det_fps, int current_cap_fps)
{
    int const colors[6][3] = { { 1,0,1 },{ 0,0,1 },{ 0,1,1 },{ 0,1,0 },{ 1,1,0 },{ 1,0,0 } };

    for (auto &i : result_vec) {
        Scalar color = obj_id_to_color(i.obj_id);
        rectangle(mat_img, Rect(i.x, i.y, i.w, i.h), color, 2);
        if (obj_names.size() > i.obj_id) {
            string obj_name = obj_names[i.obj_id];
            if (i.track_id > 0) obj_name += " - " + to_string(i.track_id);
            Size const text_size = getTextSize(obj_name, FONT_HERSHEY_COMPLEX_SMALL, 1.2, 2, 0);
            int const max_width = (text_size.width > i.w + 2) ? text_size.width : (i.w + 2);
            rectangle(mat_img, Point2f(max((int)i.x - 1, 0), max((int)i.y - 30, 0)),
                Point2f(min((int)i.x + max_width, mat_img.cols-1), min((int)i.y, mat_img.rows-1)),
                color, CV_FILLED, 8, 0);
            putText(mat_img, obj_name, Point2f(i.x, i.y - 10), FONT_HERSHEY_COMPLEX_SMALL, 1.2, Scalar(0, 0, 0), 2);
        }
    }
    if (current_det_fps >= 0 && current_cap_fps >= 0) {
        string fps_str = "FPS detection: " + to_string(current_det_fps) + "   FPS capture: " + to_string(current_cap_fps);
        putText(mat_img, fps_str, Point2f(10, 20), FONT_HERSHEY_COMPLEX_SMALL, 1.2, Scalar(50, 255, 0), 2);
    }
}

vector<string> DarknetDetector::objects_names_from_file(string const filename)
{
    ifstream file(filename);
    vector<string> file_lines;
    if (!file.is_open()) return file_lines;
    for(string line; getline(file, line);) file_lines.push_back(line);
    cout << "object names loaded \n";
    return file_lines;
}

bool DarknetDetector::detect_image(Mat image)
{
    return detect(image);
}

bool DarknetDetector::detect_image(QImage image)
{
    Mat mat = this->QImage2cvMat(image);

    return detect(mat);
}

void DarknetDetector::init_model(QString names_file, QString cfg_file, QString weights_file)
{
    string names_file_string = names_file.toStdString();
    string cfg_file_string = cfg_file.toStdString();
    string weights_file_string = weights_file.toStdString();

    //obj_names = objects_names_from_file(names_file); //调用获得分类对象名称
    //或者使用以下四行代码也可实现读入分类对象文件
    ifstream ifs(names_file_string.c_str());
    string line;

    darknet_detector_ = new Detector(cfg_file_string, weights_file_string, 0);

    obj_names_.clear();

    while (getline(ifs, line))
    {
        obj_names_.push_back(line);
    }
}

QImage DarknetDetector::cvMat2QImage(const Mat& mat)
{
    // 8-bits unsigned, NO. OF CHANNELS = 1
    if(mat.type() == CV_8UC1)
    {
        QImage image(mat.cols, mat.rows, QImage::Format_Indexed8);
        // Set the color table (used to translate colour indexes to qRgb values)
        image.setColorCount(256);
        for(int i = 0; i < 256; i++)
        {
            image.setColor(i, qRgb(i, i, i));
        }
        // Copy input Mat
        uchar *pSrc = mat.data;
        for(int row = 0; row < mat.rows; row ++)
        {
            uchar *pDest = image.scanLine(row);
            memcpy(pDest, pSrc, mat.cols);
            pSrc += mat.step;
        }
        return image;
    }
    // 8-bits unsigned, NO. OF CHANNELS = 3
    else if(mat.type() == CV_8UC3)
    {
        // Copy input Mat
        const uchar *pSrc = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
        return image.rgbSwapped();
    }
    else if(mat.type() == CV_8UC4)
    {
        qDebug() << "CV_8UC4";
        // Copy input Mat
        const uchar *pSrc = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);
        return image.copy();
    }
    else
    {
        qDebug() << "ERROR: Mat could not be converted to QImage.";
        return QImage();
    }
}

Mat DarknetDetector::QImage2cvMat(QImage image)
{
    Mat mat;
    switch(image.format())
    {
    case QImage::Format_ARGB32:
    case QImage::Format_RGB32:
    case QImage::Format_ARGB32_Premultiplied:
        mat = Mat(image.height(), image.width(), CV_8UC4, (void*)image.constBits(), image.bytesPerLine());
        break;
    case QImage::Format_RGB888:
        mat = Mat(image.height(), image.width(), CV_8UC3, (void*)image.constBits(), image.bytesPerLine());
        cvtColor(mat, mat, CV_BGR2RGB);
        break;
    case QImage::Format_Indexed8:
        mat = Mat(image.height(), image.width(), CV_8UC1, (void*)image.constBits(), image.bytesPerLine());
        break;
    case QImage::Format_Grayscale8:
        mat = Mat(image.height(), image.width(), CV_8UC1, (void*)image.constBits(), image.bytesPerLine());
        break;
    }
    return mat;
}

bool DarknetDetector::detect(Mat image)
{
    if(detector_loaded_)
    {
        result_vec_ = darknet_detector_->detect(image);

        return true;
    }

    return false;
}
