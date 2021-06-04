#include "DarknetDetector.h"

#define OPENCV
#define GPU

using namespace cv;

#ifdef WIN32
#pragma comment(lib, "L:/Project/ABACI/FilterSocket/bin_win/darknet-win-gpu/yolo_cpp_dll.lib")//引入链接库

DarknetDetector::DarknetDetector(QObject *parent) : QObject (parent)
{
    region_detector_loaded_ = false;
    component_detector_loaded_ = false;

    bool detect_2c = false;
    bool detect_4c = false;
    bool detect_waterDrop = true;

    if(detect_2c)
    {
//        //Load Region Detector [每张图标注一个区域]
//        QString region_names_file = "D:/chLi/Project/ABACI/FilterSocket/bin_win/darknet-win-gpu/yolov3_train_2c_region_1class/coco.names";
//        QString region_cfg_file = "D:/chLi/Project/ABACI/FilterSocket/bin_win/darknet-win-gpu/yolov3_train_2c_region_1class/yolov3.cfg";
//        QString region_weights_file = "D:/chLi/Project/ABACI/FilterSocket/bin_win/darknet-win-gpu/yolov3_train_2c_region_1class/yolov3_train_2c_region_1class.backup";

//        init_model(Yolov3Region, region_names_file, region_cfg_file, region_weights_file);

//        region_detector_loaded_ = true;

//        qDebug() << "init region detector finished.";

        //Load Region Detector [每张图标注前两排立柱]
        QString region_names_file = "D:/chLi/Project/ABACI/FilterSocket/bin_win/darknet-win-gpu/yolov3_train_2c_region_2row_mask_1class/coco.names";
        QString region_cfg_file = "D:/chLi/Project/ABACI/FilterSocket/bin_win/darknet-win-gpu/yolov3_train_2c_region_2row_mask_1class/yolov3.cfg";
        QString region_weights_file = "D:/chLi/Project/ABACI/FilterSocket/bin_win/darknet-win-gpu/yolov3_train_2c_region_2row_mask_1class/yolov3_train_2c_region_2row_mask_1class.backup";

        init_model(Yolov3Region, region_names_file, region_cfg_file, region_weights_file);

        region_detector_loaded_ = true;

        qDebug() << "init region detector finished.";

        //Load Component Detetor
        QString component_names_file = "D:/chLi/Project/ABACI/FilterSocket/bin_win/darknet-win-gpu/yolov3_train_2c_detect_2class/coco.names";
        QString component_cfg_file = "D:/chLi/Project/ABACI/FilterSocket/bin_win/darknet-win-gpu/yolov3_train_2c_detect_2class/yolov3.cfg";
        QString component_weights_file = "D:/chLi/Project/ABACI/FilterSocket/bin_win/darknet-win-gpu/yolov3_train_2c_detect_2class/yolov3_train_2c_detect_2class.backup";

        init_model(Yolov3Component, component_names_file, component_cfg_file, component_weights_file);

        component_detector_loaded_ = true;

        qDebug() << "init component detector finished.";
    }

    if(detect_4c)
    {
        //Load Region Detector
        QString region_names_file = "D:/chLi/Project/ABACI/FilterSocket/bin_win/darknet-win-gpu/yolov3_train_4c_jueyuanzi_32class/coco.names";
        QString region_cfg_file = "D:/chLi/Project/ABACI/FilterSocket/bin_win/darknet-win-gpu/yolov3_train_4c_jueyuanzi_32class/yolov3.cfg";
        QString region_weights_file = "D:/chLi/Project/ABACI/FilterSocket/bin_win/darknet-win-gpu/yolov3_train_4c_jueyuanzi_32class/yolov3_train_4c_jueyuanzi_32class.backup";

        init_model(Yolov3Region, region_names_file, region_cfg_file, region_weights_file);

        region_detector_loaded_ = true;

        qDebug() << "init region detector finished.";
    }

    if(detect_waterDrop)
    {
        //Load Region Detector
        QString region_names_file = "D:/chLi/Project/ABACI/FilterSocket/bin_win/darknet-win-gpu/yolov3_train_waterDrop_2class/coco.names";
        QString region_cfg_file = "D:/chLi/Project/ABACI/FilterSocket/bin_win/darknet-win-gpu/yolov3_train_waterDrop_2class/yolov3.cfg";
        QString region_weights_file = "D:/chLi/Project/ABACI/FilterSocket/bin_win/darknet-win-gpu/yolov3_train_waterDrop_2class/yolov3_train_waterDrop_2class.backup";

        init_model(Yolov3Region, region_names_file, region_cfg_file, region_weights_file);

        region_detector_loaded_ = true;

        qDebug() << "init region detector finished.";
    }
}

DarknetDetector::~DarknetDetector()
{
    delete region_detector_;
    delete component_detector_;
}

void DarknetDetector::slot_LoadImage(QString image_path)
{
    mat_ = imread(image_path.toStdString());
    QImage disImage;
    disImage= QImage((const unsigned char*)(mat_.data), mat_.cols, mat_.rows, QImage::Format_RGB888);

    std::vector<bbox_t> result_vec = region_detector_->detect(mat_);
    draw_boxes(mat_, result_vec, region_obj_names_);

    cv::namedWindow("test", CV_WINDOW_NORMAL);
    cv::imshow("test", mat_);
    cv::waitKey(3);
}

void DarknetDetector::slot_LoadVideo(QString video_path)
{
    capture_.open(video_path.toStdString());
    if (!capture_.isOpened())
    {
        printf("文件打开失败");
    }
    cv::Mat frame;

    while (true)
    {
        capture_ >> frame;
        std::vector<bbox_t> result_vec = region_detector_->detect(frame);
        draw_boxes(frame, result_vec, region_obj_names_);

        cv::namedWindow("test", CV_WINDOW_NORMAL);
        cv::imshow("test", frame);
        cv::waitKey(3);
    }
}

QImage DarknetDetector::cvMat2QImage(const cv::Mat& mat)
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

cv::Mat DarknetDetector::QImage2cvMat(QImage image)
{
    cv::Mat mat;
    switch(image.format())
    {
    case QImage::Format_ARGB32:
    case QImage::Format_RGB32:
    case QImage::Format_ARGB32_Premultiplied:
        mat = cv::Mat(image.height(), image.width(), CV_8UC4, (void*)image.constBits(), image.bytesPerLine());
        break;
    case QImage::Format_RGB888:
        mat = cv::Mat(image.height(), image.width(), CV_8UC3, (void*)image.constBits(), image.bytesPerLine());
        cv::cvtColor(mat, mat, CV_BGR2RGB);
        break;
    case QImage::Format_Indexed8:
        mat = cv::Mat(image.height(), image.width(), CV_8UC1, (void*)image.constBits(), image.bytesPerLine());
        break;
    case QImage::Format_Grayscale8:
        mat = cv::Mat(image.height(), image.width(), CV_8UC1, (void*)image.constBits(), image.bytesPerLine());
        break;
    }
    return mat;
}

void DarknetDetector::draw_boxes(cv::Mat mat_img, std::vector<bbox_t> result_vec, std::vector<std::string> obj_names,
    int current_det_fps, int current_cap_fps)
{
    int const colors[6][3] = { { 1,0,1 },{ 0,0,1 },{ 0,1,1 },{ 0,1,0 },{ 1,1,0 },{ 1,0,0 } };

    for (auto &i : result_vec) {
        cv::Scalar color = obj_id_to_color(i.obj_id);
        cv::rectangle(mat_img, cv::Rect(i.x, i.y, i.w, i.h), color, 2);
        if (obj_names.size() > i.obj_id) {
            std::string obj_name = obj_names[i.obj_id];
            if (i.track_id > 0) obj_name += " - " + std::to_string(i.track_id);
            cv::Size const text_size = getTextSize(obj_name, cv::FONT_HERSHEY_COMPLEX_SMALL, 1.2, 2, 0);
            int const max_width = (text_size.width > i.w + 2) ? text_size.width : (i.w + 2);
            cv::rectangle(mat_img, cv::Point2f(std::max((int)i.x - 1, 0), std::max((int)i.y - 30, 0)),
                cv::Point2f(std::min((int)i.x + max_width, mat_img.cols-1), std::min((int)i.y, mat_img.rows-1)),
                color, CV_FILLED, 8, 0);
            putText(mat_img, obj_name, cv::Point2f(i.x, i.y - 10), cv::FONT_HERSHEY_COMPLEX_SMALL, 1.2, cv::Scalar(0, 0, 0), 2);
        }
    }
    if (current_det_fps >= 0 && current_cap_fps >= 0) {
        std::string fps_str = "FPS detection: " + std::to_string(current_det_fps) + "   FPS capture: " + std::to_string(current_cap_fps);
        putText(mat_img, fps_str, cv::Point2f(10, 20), cv::FONT_HERSHEY_COMPLEX_SMALL, 1.2, cv::Scalar(50, 255, 0), 2);
    }
}

std::vector<std::string> DarknetDetector::objects_names_from_file(std::string const filename)
{
    std::ifstream file(filename);
    std::vector<std::string> file_lines;
    if (!file.is_open()) return file_lines;
    for(std::string line; getline(file, line);) file_lines.push_back(line);
    std::cout << "object names loaded \n";
    return file_lines;
}

std::vector<bbox_t> DarknetDetector::getRegionResult(cv::Mat image)
{
    std::vector<bbox_t> region_result_vec;

    if(region_detector_loaded_)
    {
        region_result_vec = region_detector_->detect(image);
    }

    return region_result_vec;
}

std::vector<bbox_t> DarknetDetector::getRegionResult(QImage image)
{
    std::vector<bbox_t> region_result_vec;

    if(region_detector_loaded_)
    {
        Mat region_mat = this->QImage2cvMat(image);

        region_result_vec = region_detector_->detect(region_mat);
    }

    return region_result_vec;
}

std::vector<bbox_t> DarknetDetector::getComponentResult(Mat image)
{
    std::vector<bbox_t> component_result_vec;

    if(component_detector_loaded_)
    {
        component_result_vec = component_detector_->detect(image);
    }

    return component_result_vec;
}

std::vector<bbox_t> DarknetDetector::getComponentResult(QImage image)
{
    std::vector<bbox_t> component_result_vec;

    if(component_detector_loaded_)
    {
        Mat component_mat = this->QImage2cvMat(image);

        component_result_vec = component_detector_->detect(component_mat);
    }

    return component_result_vec;
}

std::vector<bbox_t> DarknetDetector::getDarknetResult(cv::Mat mat)
{
    QImage image = this->cvMat2QImage(mat);

    return getDarknetResult(image);
}

std::vector<bbox_t> DarknetDetector::getDarknetResult(QImage image)
{
    std::vector<bbox_t> result_vec;

    std::vector<bbox_t> region_result_vec;

    std::vector<bbox_t> component_result_vec;

    if(region_detector_loaded_)
    {
        cv::Mat region_mat = this->QImage2cvMat(image);

        region_result_vec = region_detector_->detect(region_mat);

        result_vec = region_result_vec;
    }

    if(component_detector_loaded_)
    {
        if(region_result_vec.size() > 0)
        {
            for(bbox_t bbox : region_result_vec)
            {
                QImage component_image = image.copy(bbox.x, bbox.y, bbox.w, bbox.h);

                cv::Mat component_mat = this->QImage2cvMat(component_image);

                std::vector<bbox_t> current_component_result_vec;

                current_component_result_vec = component_detector_->detect(component_mat);

                if(current_component_result_vec.size() > 0)
                {
                    for(bbox_t component_bbox : current_component_result_vec)
                    {
                        component_result_vec.push_back(component_bbox);
                    }
                }
            }
        }

        result_vec = component_result_vec;
    }

    return result_vec;
}

void DarknetDetector::init_model(NetworkType network_type, QString names_file, QString cfg_file, QString weights_file)
{
    std::string names_file_string = names_file.toStdString();
    std::string cfg_file_string = cfg_file.toStdString();
    std::string weights_file_string = weights_file.toStdString();

    //obj_names = objects_names_from_file(names_file); //调用获得分类对象名称
    //或者使用以下四行代码也可实现读入分类对象文件
    std::ifstream ifs(names_file_string.c_str());
    std::string line;

    switch(network_type)
    {
    case Yolov3Region:

        region_detector_ = new Detector(cfg_file_string, weights_file_string,0);

        region_obj_names_.clear();

        while (getline(ifs, line))
        {
            region_obj_names_.push_back(line);
        }

        break;
    case Yolov3Component:

        component_detector_ = new Detector(cfg_file_string, weights_file_string,0);

        component_obj_names_.clear();

        while (getline(ifs, line))
        {
            component_obj_names_.push_back(line);
        }

        break;
    case ResNetClassification:
        break;
    }
}

bool DarknetDetector::darknet_process(Connection::ConnectedData &data)
{
    QImage img;

    data.toQImage(img);

    // process

    clock_t start = clock();
    std::vector<bbox_t> result = this->getDarknetResult(img);
    std::cout << int((clock() - start)) << " ms" << std::endl;

    data.resetBBoxNum(result.size());

    for(int i=0; i < data.info.bbox_num_; i++)
    {
        data.setBBox(i, result[i].x, result[i].y, result[i].w, result[i].h, result[i].obj_id);
    }

    return true;
}

void DarknetDetector::slot_detect(Connection::ConnectedData& data)
{
    darknet_process(data);

    emit signal_detect_finished();
}
#endif

#ifdef Linux
Detector::Detector(const std::string &yolov3_cfg, const std::string &yolov3_weights, const std::string &coco_cfg, QObject *parent)
{
    const char *c_str_yolov3_cfg = yolov3_cfg.c_str();
    const char *c_str_yolov3_weights = yolov3_weights.c_str();
    const char *c_str_coco_cfg = coco_cfg.c_str();

    char *str_yolov3_cfg = new char[strlen(c_str_yolov3_cfg) + 1];
    char *str_yolov3_weights = new char[strlen(c_str_yolov3_weights) + 1];
    char *str_coco_cfg = new char[strlen(c_str_coco_cfg) + 1];

    strcpy(str_yolov3_cfg, c_str_yolov3_cfg);
    strcpy(str_yolov3_weights, c_str_yolov3_weights);
    strcpy(str_coco_cfg, c_str_coco_cfg);

    cuda_set_device(gpu_index);

    net = load_network(str_yolov3_cfg, str_yolov3_weights, 0);

    meta = get_metadata(str_coco_cfg);
}

Detector::~Detector()
{
    delete net;
}

std::vector<std::pair<char *, std::vector<float>>> Detector::detect(image im, float thresh, float hier_thresh, float nms)
{
    int num = 0;

    int *pnum = &num;

    network_predict_image(net, im);

    detection *dets = get_network_boxes(net, im.w, im.h, thresh, hier_thresh, nullptr, 0, pnum);

    num = pnum[0];

    if (nms)
    {
        do_nms_obj(dets, num, meta.classes, nms);
    }

    std::vector<std::pair<char *, std::vector<float>>> res;

    for(int j = 0; j < num; ++j)
    {
        for(int i = 0; i < meta.classes; ++i)
        {
            if(dets[j].prob[i] > 0)
            {
                box b = dets[j].bbox;

                std::pair<char *, std::vector<float>> temp_data;

                temp_data.first = meta.names[i];

                temp_data.second.emplace_back(dets[j].prob[i]);
                temp_data.second.emplace_back(b.x - b.w / 2.0);
                temp_data.second.emplace_back(b.y - b.h / 2.0);
                temp_data.second.emplace_back(b.w);
                temp_data.second.emplace_back(b.h);
                temp_data.second.emplace_back(i);

                res.emplace_back(temp_data);
            }
        }
    }

    if(res.size() > 1)
    {
        for(int i = 0; i < res.size() - 1; ++i)
        {
            for(int j = i + 1; j < res.size(); ++j)
            {
                if(res[i].second[0] < res[j].second[0])
                {
                    std::pair<char *, std::vector<float>> exchange_data = res[i];

                    res[i] = res[j];
                    res[j] = exchange_data;
                }
            }
        }
    }

    free_image(im);

    free_detections(dets, num);

    return res;
}

std::vector<std::pair<char *, std::vector<float>>> Detector::detect(char *img, float thresh, float hier_thresh, float nms)
{
    image im = load_image_color(img, 0, 0);

    return detect(im, thresh, hier_thresh, nms);
}

std::vector<std::pair<char *, std::vector<float>>> Detector::detect(float *img, int w, int h, int c, float thresh, float hier_thresh, float nms)
{
    image im;
    im.w = w;
    im.h = h;
    im.c = c;

    int im_size = w * h * c;

    im.data = new float[im_size];

    memcpy(im.data, img, im_size * sizeof(float));

    return detect(im, thresh, hier_thresh, nms);
}

DarknetDetector::DarknetDetector(QObject *parent) : QObject (parent)
{
    region_detector_loaded_ = false;
    component_detector_loaded_ = false;

    bool detect_2c = true;
    bool detect_4c = false;

    if(detect_2c)
    {
//        //Load Region Detector [每张图标注一个区域]
//        QString region_names_file = "/home/abaci/chLi/FilterSocket/bin_win/darknet-win-gpu/yolov3_train_2c_region_1class/coco.names";
//        QString region_cfg_file = "/home/abaci/chLi/FilterSocket/bin_win/darknet-win-gpu/yolov3_train_2c_region_1class/yolov3.cfg";
//        QString region_weights_file = "/home/abaci/chLi/FilterSocket/bin_win/darknet-win-gpu/yolov3_train_2c_region_1class/yolov3_train_2c_region_1class.backup";

//        init_model(Yolov3Region, region_names_file, region_cfg_file, region_weights_file);

//        region_detector_loaded_ = true;

//        qDebug() << "init region detector finished.";

        //Load Region Detector [每张图标注前两排立柱]
        QString region_coco_file = "/home/abaci/chLi/FilterSocket/bin_win/darknet-win-gpu/yolov3_train_2c_region_2row_mask_1class/coco.data";
        QString region_cfg_file = "/home/abaci/chLi/FilterSocket/bin_win/darknet-win-gpu/yolov3_train_2c_region_2row_mask_1class/yolov3.cfg";
        QString region_weights_file = "/home/abaci/chLi/FilterSocket/bin_win/darknet-win-gpu/yolov3_train_2c_region_2row_mask_1class/yolov3_train_2c_region_2row_mask_1class.backup";

        init_model(Yolov3Region, region_coco_file, region_cfg_file, region_weights_file);

        region_detector_loaded_ = true;

        qDebug() << "init region detector finished.";

        //Load Component Detetor
        QString component_coco_file = "/home/abaci/chLi/FilterSocket/bin_win/darknet-win-gpu/yolov3_train_2c_detect_2class/coco.data";
        QString component_cfg_file = "/home/abaci/chLi/FilterSocket/bin_win/darknet-win-gpu/yolov3_train_2c_detect_2class/yolov3.cfg";
        QString component_weights_file = "/home/abaci/chLi/FilterSocket/bin_win/darknet-win-gpu/yolov3_train_2c_detect_2class/yolov3_train_2c_detect_2class.backup";

        init_model(Yolov3Component, component_coco_file, component_cfg_file, component_weights_file);

        component_detector_loaded_ = true;

        qDebug() << "init component detector finished.";
    }

    if(detect_4c)
    {
        //Load Region Detector
        QString region_coco_file = "/home/abaci/chLi/FilterSocket/bin_win/darknet-win-gpu/yolov3_train_4c_jueyuanzi_32class/coco.data";
        QString region_cfg_file = "/home/abaci/chLi/FilterSocket/bin_win/darknet-win-gpu/yolov3_train_4c_jueyuanzi_32class/yolov3.cfg";
        QString region_weights_file = "/home/abaci/chLi/FilterSocket/bin_win/darknet-win-gpu/yolov3_train_4c_jueyuanzi_32class/yolov3_train_4c_jueyuanzi_32class.backup";

        init_model(Yolov3Region, region_coco_file, region_cfg_file, region_weights_file);

        region_detector_loaded_ = true;

        qDebug() << "init region detector finished.";
    }
}

DarknetDetector::~DarknetDetector()
{
    delete region_detector_;
    delete component_detector_;
}

std::vector<std::pair<char *, std::vector<float> > > DarknetDetector::getRegionResult(float *img, int w, int h, int c, float thresh, float hier_thresh, float nms)
{
    return region_detector_->detect(img, w, h, c, thresh, hier_thresh, nms);
}

std::vector<std::pair<char *, std::vector<float> > > DarknetDetector::getComponentResult(float *img, int w, int h, int c, float thresh, float hier_thresh, float nms)
{
    return component_detector_->detect(img, w, h, c, thresh, hier_thresh, nms);
}

std::vector<std::pair<char *, std::vector<float>>> DarknetDetector::getDarknetResult(image img, float thresh, float hier_thresh, float nms)
{
    return region_detector_->detect(img, thresh, hier_thresh, nms);
}

std::vector<std::pair<char *, std::vector<float>>> DarknetDetector::getDarknetResult(char *img, float thresh, float hier_thresh, float nms)
{
    return region_detector_->detect(img, thresh, hier_thresh, nms);
}

std::vector<std::pair<char *, std::vector<float>>> DarknetDetector::getDarknetResult(float *img, int w, int h, int c, float thresh, float hier_thresh, float nms)
{
    return region_detector_->detect(img, w, h, c, thresh, hier_thresh, nms);
}

void DarknetDetector::init_model(NetworkType network_type, QString coco_file, QString cfg_file, QString weights_file)
{
    std::string str_coco_file = coco_file.toStdString();
    std::string str_cfg_file = cfg_file.toStdString();
    std::string str_weights_file = weights_file.toStdString();

    switch(network_type)
    {
    case Yolov3Region:

        region_detector_ = new Detector(str_cfg_file, str_weights_file, str_coco_file);

        break;
    case Yolov3Component:

        component_detector_ = new Detector(str_cfg_file, str_weights_file, str_coco_file);

        break;
    case ResNetClassification:
        break;
    }
}
#endif
