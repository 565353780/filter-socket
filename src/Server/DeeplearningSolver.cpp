#include "DeeplearningSolver.h"

namespace FilterSocket {

DeeplearningSolver::DeeplearningSolver()
{
    video_cap_start = false;
}

void DeeplearningSolver::init()
{    
    QString conda_env_name = "";

#ifdef WIN32
    conda_env_name = "py37";
#endif
#ifdef Linux
    conda_env_name = "pytorch";
#endif

    bool load_lapnet = false;
    bool load_darknet = false;

    if(load_lapnet)
    {
        if(conda_env_name != "")
        {
    #ifdef WIN32
            QString current_path = QDir::currentPath();

            QString current_disk_id = current_path.split(":")[0];

            qDebug() << ("start conda activate " + conda_env_name + "&&" + current_disk_id + ":&&cd " + current_path + "&&python ../src/Python/LapNet/test.py&&exit").toStdString().c_str();

            system(("start conda activate " + conda_env_name + "&&" + current_disk_id + ":&&cd " + current_path + "&&python ../src/Python/LapNet/test.py&&exit").toStdString().c_str());
    #endif
    #ifdef Linux
            system(("RUN_AFTER_BASHRC=\"conda activate " + conda_env_name + "&&python ../src/Python/LapNet/test.py&&exit\" gnome-terminal").toStdString().c_str());
    #endif
        }
        else
        {
    #ifdef WIN32
            system("start python ../src/Python/LapNet/test.py&&exit");
    #endif
    #ifdef Linux
            system("RUN_AFTER_BASHRC=\"python ../src/Python/LapNet/test.py&&exit\" gnome-terminal");
    #endif
        }
    }

    QDir dir("../src/Python/LapNet/trained_model");
    QString srcDirPath = dir.absolutePath();

    QFileInfoList file_list = Connection::GetFileList(srcDirPath);

    int max_episode_idx = -1;
    int max_episode_num = -1;

    for(int i = 0; i < file_list.size(); ++i)
    {
        QString filename = file_list[i].fileName();

        int current_episode_num = filename.split("LapNet_chkpt_better_epoch")[1].split("_")[0].toInt();

        if(current_episode_num > max_episode_num)
        {
            max_episode_num = current_episode_num;
            max_episode_idx = i;
        }
    }

//    if(max_episode_idx > -1)
//    {
//        lapnet_model = file_list[max_episode_idx].absoluteFilePath().toStdString().c_str();
//    }

    auto lapnet_model = "../models/lapnet/LapNet_test.xml";

    auto yolov3_model = "../models/yolov3/yolov3_test.xml";

    auto yolov3_cfg = "../Server_DataBase/train_dataset/darknet_dataset/yolov3_test.cfg";

    auto yolov3_weights = "../Linux/darknet-master/backup/yolov3_train.backup";
    //yolov3_weights = "../Linux/darknet-master/backup/yolov3.weights";

    auto coco_cfg = "../Server_DataBase/train_dataset/darknet_dataset/coco.data";

    QFile file_yolo(yolov3_cfg);
    if(!file_yolo.exists())
    {
        yolov3_cfg = "../models/darknet/yolov3.cfg";

        yolov3_weights = "../models/darknet/yolov3_17100.weights";

        coco_cfg = "../models/darknet/coco.data";
    }

    qDebug() << "init model finished!" << endl;

    data_rebuild_solver = new DataRebuild();
    lapnet_train_solver = new LapNet_Train();
}

void DeeplearningSolver::init_model(QString conda_env_name)
{
    QFile file_lapnet_stop("../Server_DataBase/message_set/lapnet_test_stopped.txt");

    if(file_lapnet_stop.open(QFile::WriteOnly))
    {
        file_lapnet_stop.resize(0);
        file_lapnet_stop.write("True");
        file_lapnet_stop.close();
    }

    if(conda_env_name != "")
    {
#ifdef WIN32
        system(("start conda activate " + conda_env_name + "&&python ../src/Python/LapNet/test.py&&exit").toStdString().c_str());
#endif
#ifdef Linux
        system(("RUN_AFTER_BASHRC=\"conda activate " + conda_env_name + "&&python ../src/Python/LapNet/test.py&&exit\" gnome-terminal").toStdString().c_str());
#endif
    }
    else
    {
#ifdef WIN32
        system("start python ../src/Python/LapNet/test.py&&exit");
#endif
#ifdef Linux
        system("RUN_AFTER_BASHRC=\"python ../src/Python/LapNet/test.py&&exit\" gnome-terminal");
#endif
    }

    QDir dir("../src/Python/LapNet/trained_model");
    QString srcDirPath = dir.absolutePath();

    QFileInfoList file_list = Connection::GetFileList(srcDirPath);

    int max_episode_idx = -1;
    int max_episode_num = -1;

    for(int i = 0; i < file_list.size(); ++i)
    {
        QString filename = file_list[i].fileName();

        int current_episode_num = filename.split("LapNet_chkpt_better_epoch")[1].split("_")[0].toInt();

        if(current_episode_num > max_episode_num)
        {
            max_episode_num = current_episode_num;
            max_episode_idx = i;
        }
    }

//    if(max_episode_idx > -1)
//    {
//        lapnet_model = file_list[max_episode_idx].absoluteFilePath().toStdString().c_str();
//    }

    auto lapnet_model = "../models/lapnet/LapNet_test.xml";

    auto yolov3_model = "../models/yolov3/yolov3_test.xml";

    auto yolov3_cfg = "../Server_DataBase/train_dataset/darknet_dataset/yolov3_test.cfg";

    auto yolov3_weights = "../Linux/darknet-master/backup/yolov3_train.backup";
    //yolov3_weights = "../Linux/darknet-master/backup/yolov3.weights";

    auto coco_cfg = "../Server_DataBase/train_dataset/darknet_dataset/coco.data";

    QFile file_yolo(yolov3_cfg);
    if(!file_yolo.exists())
    {
        yolov3_cfg = "../models/darknet/yolov3.cfg";

        yolov3_weights = "../models/darknet/yolov3_17100.weights";

        coco_cfg = "../models/darknet/coco.data";
    }

    qDebug() << "init model finished!" << endl;
}

void DeeplearningSolver::close_server()
{
    QDir dir_;

    QFile file_json("../Server_DataBase/message_set/server.json");

    if(!dir_.exists("../Server_DataBase/message_set/server.json"))
    {
        if(!file_json.open(QFile::WriteOnly))
        {
            qDebug() << "main : server.json write failed.";
            return;
        }

        QJsonObject obj;
        obj["server_started"] = true;
        obj["load_net"] = false;
        obj["thresh"] = 0.5;

        QJsonDocument doc;
        doc.setObject(obj);

        QByteArray array;
        array.push_back(doc.toJson());

        file_json.write(array.toBase64());

        file_json.close();
    }
    else
    {
        if(!file_json.open(QFile::ReadOnly))
        {
            qDebug() << "main : server.json read failed.";
            return;
        }

        QByteArray array = QByteArray::fromBase64(file_json.readAll());

        QJsonDocument doc = QJsonDocument::fromJson(array);

        QJsonObject obj = doc.object();

        if(!obj["server_started"].toBool())
        {
            obj["server_started"] = true;
        }

        file_json.close();
    }

    QFile file_lapnet_stop("../Server_DataBase/message_set/lapnet_test_stopped.txt");

    if(file_lapnet_stop.open(QFile::WriteOnly))
    {
        file_lapnet_stop.resize(0);
        file_lapnet_stop.write("True");
        file_lapnet_stop.close();
    }

    QFile file_server_stop("../Server_DataBase/message_set/server_stopped.txt");

    if(file_server_stop.open(QFile::WriteOnly))
    {
        file_server_stop.resize(0);
        file_server_stop.write("True");
        file_server_stop.close();
    }

    exit(0);
}

int DeeplearningSolver::lapnet_process(Connection::ConnectedData &data)
{
    cv::Mat frame(data.info.img_height_,data.info.img_width_,CV_8UC3);

    for(int i=0; i < data.info.img_width_; i++)
    {
        for(int j=0; j < data.info.img_height_; j++)
        {
            // pixel(i,j)
            frame.at<cv::Vec3b>(j,i)[0] = data.getBlue(i,j);
            frame.at<cv::Vec3b>(j,i)[1] = data.getGreen(i,j);
            frame.at<cv::Vec3b>(j,i)[2] = data.getRed(i,j);
        }
    }

    // process

    //cv::Mat result = lapnet_detector->getLapNetResult(frame);

    cv::imwrite((QDir::currentPath() + "/../Server_DataBase/message_set/lapnet_eval.jpg").toStdString(), frame);

    QFile img_send_finished(QDir::currentPath() + "/../Server_DataBase/message_set/lapnet_eval_finish.txt");

    if(img_send_finished.open(QFile::WriteOnly))
    {
        img_send_finished.write("True");

        img_send_finished.close();
    }

    QFile file(QDir::currentPath() + "/../Server_DataBase/message_set/lapnet_eval.jpg");

    while(file.exists())
    {
        continue;
    }

    cv::Mat result = cv::imread((QDir::currentPath() + "/../Server_DataBase/message_set/lapnet_eval_out.jpg").toStdString());

    cv::resize(result, result, cv::Size(data.info.img_width_, data.info.img_height_));

    for(int i=0; i < data.info.img_width_; i++)
    {
        for(int j=0; j < data.info.img_height_; j++)
        {
//            if(result.at<cv::Vec3b>(j,i)[0] + result.at<cv::Vec3b>(j,i)[1] + result.at<cv::Vec3b>(j,i)[2] < 10)
//            {
//                data.setPixel(i,j, 0, 0, 0);
//            }
//            else
//            {
//                data.setPixel(i,j,255, 255, 255); // 更新数据
//            }
            data.setPixel(i,j, result.at<cv::Vec3b>(j,i)[0], result.at<cv::Vec3b>(j,i)[1], result.at<cv::Vec3b>(j,i)[2]);
        }
    }

    return NoError;
}

#ifdef WIN32
int DeeplearningSolver::darknet_process(Connection::ConnectedData &data)
{
    QImage img;

    data.toQImage(img);

    // process

    clock_t start = clock();
    std::vector<bbox_t> result = darknet_detector->getRegionResult(img);
    std::cout << int((clock() - start)) << " ms" << std::endl;

    data.resetBBoxNum(result.size());

    for(int i=0; i < data.info.bbox_num_; i++)
    {
        data.setBBox(i, result[i].x, result[i].y, result[i].w, result[i].h, result[i].obj_id);
    }

    return NoError;
}

int DeeplearningSolver::darknet_test(Connection::ConnectedData &data)
{
    bool detect_2c = true;
    bool detect_4c = false;

    QString data_path = data.info.image_dir_;
    QString catch_data_path = data.info.output_dir_;

    //Test Zone
    if(detect_2c)
    {
        QDir dir_;

        float catch_fault_prob = 0.9;

        QString data_path_copy = data_path;
        catch_data_path += data_path_copy.replace(":", "").replace(".jpg", "").replace("/", "_") + "/";

        QString no_error_path = catch_data_path + "正常/正常/";//没有检测到目标 ： 正常
        QString found_true_error_path = catch_data_path + "异常/异常/";//接触网区域内检测到目标 ： 异常
        QString found_false_error_path = catch_data_path + "正常/背景异常/";//接触网区域外检测到目标 ： 背景异常
        QString found_unknown_error_path = catch_data_path + "异常/疑似异常/";//未检测到接触网区域但检测到目标 + 接触网区域内检测到目标的得分 < 0.9 ： 疑似异常

        if(!dir_.exists(catch_data_path))
        {
            dir_.mkpath(catch_data_path);
        }

        if(!dir_.exists(no_error_path))
        {
            dir_.mkpath(no_error_path);
        }
        if(!dir_.exists(found_true_error_path))
        {
            dir_.mkpath(found_true_error_path);
        }
        if(!dir_.exists(found_false_error_path))
        {
            dir_.mkpath(found_false_error_path);
        }
        if(!dir_.exists(found_unknown_error_path))
        {
            dir_.mkpath(found_unknown_error_path);
        }

        MaskDatasetController *mask_controller = new MaskDatasetController();

        mask_controller->init_param(data_path, catch_data_path, -1, -1);

        mask_controller->create_mask_work_station();

        QVector<QString> file_path_list = mask_controller->get_source_file_path();

        delete mask_controller;

        int current_true_fault_num = 0;
        int current_false_fault_num = 0;
        int current_unknown_fault_num = 0;

        //2class-bird-nest
//        int bigger_pixel_scale = 100;

        for(int i = 0; i < file_path_list.size(); ++i)
        {
            clock_t start_total = clock();
            clock_t start = clock();
            QString abs_file_path = data_path + file_path_list[i];

            QString file_path_to_file_name = abs_file_path.replace(":", "").replace(".jpg", "").replace("/", "_");

            QImage source_img(data_path + file_path_list[i]);

            qDebug() << "read image to QImage : " << int((clock() - start)) << " ms";
            start = clock();

            QImage img = source_img.convertToFormat(QImage::Format_RGB888);

            qDebug() << "convertToFormat RGB888 : " << int((clock() - start)) << " ms";
            start = clock();

            cv::Mat cv_img = darknet_detector->QImage2cvMat(img);
            qDebug() << "convert to Mat : " << int((clock() - start)) << " ms";

            // process

            start = clock();
            std::vector<bbox_t> region_result = darknet_detector->getRegionResult(cv_img);

            qDebug() << "getRegionResult : " << int((clock() - start)) << " ms";
            start = clock();

            std::vector<bbox_t> component_result = darknet_detector->getComponentResult(cv_img);

            qDebug() << "getComponentResult : " << int((clock() - start)) << " ms";
            start = clock();

            std::vector<bbox_t> result;
            std::vector<bbox_t> false_result;
            std::vector<bbox_t> unknown_result;

            if(component_result.size() > 0)
            {
                for(bbox_t component_bbox : component_result)
                {
                    if(region_result.size() > 0)
                    {
                        bool is_in_region = false;

                        for(bbox_t region_bbox : region_result)
                        {
                            if(component_bbox.x + component_bbox.w < region_bbox.x ||
                                    component_bbox.x > region_bbox.x + region_bbox.w ||
                                    component_bbox.y + component_bbox.h < region_bbox.y ||
                                    component_bbox.y > region_bbox.y + region_bbox.h)
                            {
                                continue;
                            }
                            else
                            {
                                is_in_region = true;
                                break;
                            }
                        }

                        if(is_in_region)
                        {
                            result.push_back(component_bbox);
                        }
                        else
                        {
                            false_result.push_back(component_bbox);
                        }
                    }
                    else
                    {
                        unknown_result.push_back(component_bbox);
                    }
                }
                qDebug() << "compute to get true fault bbox : " << int((clock() - start)) << " ms";
            }

            start = clock();
            int line_width = 3;

            if(result.size() > 0 || false_result.size() > 0 || unknown_result.size() > 0)
            {
                if(result.size() > 0)
                {
                    QImage img_save = img.copy();

//                    QPainter painter(&img_save);

//                    painter.setCompositionMode(QPainter::CompositionMode_SourceIn);

//                    QPen pen = painter.pen();
//                    pen.setColor(Qt::red);
//                    QFont font = painter.font();
//                    font.setBold(true);
//                    font.setPixelSize(30);
//                    painter.setPen(pen);
//                    painter.setFont(font);

                    bool have_high_prob = false;

                    for(int j=0; j < result.size(); j++)
                    {
                        int x = result[j].x;
                        int y = result[j].y;
                        int w = result[j].w;
                        int h = result[j].h;
//                        int label = result[j].obj_id;
                        float prob = result[j].prob;

                        if(!have_high_prob)
                        {
                            if(prob >= catch_fault_prob)
                            {
                                have_high_prob = true;
                            }
                        }

//                        painter.drawText(x, y, "");

                        for(int k = 0; k < w; ++k)
                        {
                            for(int l = 0; l <= line_width; ++l)
                            {
                                img_save.setPixel(x + k, y + l, qRgb(255, 0, 0));
                                img_save.setPixel(x + k, y - l, qRgb(255, 0, 0));

                                img_save.setPixel(x + k, y + h + l, qRgb(255, 0, 0));
                                img_save.setPixel(x + k, y + h - l, qRgb(255, 0, 0));
                            }
                        }

                        for(int k = 0; k < h; ++k)
                        {
                            for(int l = 0; l <= line_width; ++l)
                            {
                                img_save.setPixel(x + l, y + k, qRgb(255, 0, 0));
                                img_save.setPixel(x - l, y + k, qRgb(255, 0, 0));

                                img_save.setPixel(x + w + l, y + k, qRgb(255, 0, 0));
                                img_save.setPixel(x + w - l, y + k, qRgb(255, 0, 0));
                            }
                        }
                    }

                    if(have_high_prob)
                    {
                        img_save.save(found_true_error_path + file_path_to_file_name + ".jpg", "JPG", 100);
                    }
                    else
                    {
                        img_save.save(found_unknown_error_path + file_path_to_file_name + ".jpg", "JPG", 100);
                    }

                    ++current_true_fault_num;
                }
                if(false_result.size() > 0)
                {
                    QImage img_save = img.copy();

                    for(int j=0; j < false_result.size(); j++)
                    {
                        int x = false_result[j].x;
                        int y = false_result[j].y;
                        int w = false_result[j].w;
                        int h = false_result[j].h;
//                        int label = false_result[j].obj_id;

                        for(int k = 0; k < w; ++k)
                        {
                            for(int l = 0; l <= line_width; ++l)
                            {
                                img_save.setPixel(x + k, y + l, qRgb(255, 0, 0));
                                img_save.setPixel(x + k, y - l, qRgb(255, 0, 0));

                                img_save.setPixel(x + k, y + h + l, qRgb(255, 0, 0));
                                img_save.setPixel(x + k, y + h - l, qRgb(255, 0, 0));
                            }
                        }

                        for(int k = 0; k < h; ++k)
                        {
                            for(int l = 0; l <= line_width; ++l)
                            {
                                img_save.setPixel(x + l, y + k, qRgb(255, 0, 0));
                                img_save.setPixel(x - l, y + k, qRgb(255, 0, 0));

                                img_save.setPixel(x + w + l, y + k, qRgb(255, 0, 0));
                                img_save.setPixel(x + w - l, y + k, qRgb(255, 0, 0));
                            }
                        }
                    }

                    img_save.save(found_false_error_path + file_path_to_file_name + ".jpg", "JPG", 100);

                    ++current_false_fault_num;
                }
                if(unknown_result.size() > 0)
                {
                    QImage img_save = img.copy();

                    for(int j=0; j < unknown_result.size(); j++)
                    {
                        int x = unknown_result[j].x;
                        int y = unknown_result[j].y;
                        int w = unknown_result[j].w;
                        int h = unknown_result[j].h;
//                        int label = unknown_result[j].obj_id;

                        for(int k = 0; k < w; ++k)
                        {
                            for(int l = 0; l <= line_width; ++l)
                            {
                                img_save.setPixel(x + k, y + l, qRgb(255, 0, 0));
                                img_save.setPixel(x + k, y - l, qRgb(255, 0, 0));

                                img_save.setPixel(x + k, y + h + l, qRgb(255, 0, 0));
                                img_save.setPixel(x + k, y + h - l, qRgb(255, 0, 0));
                            }
                        }

                        for(int k = 0; k < h; ++k)
                        {
                            for(int l = 0; l <= line_width; ++l)
                            {
                                img_save.setPixel(x + l, y + k, qRgb(255, 0, 0));
                                img_save.setPixel(x - l, y + k, qRgb(255, 0, 0));

                                img_save.setPixel(x + w + l, y + k, qRgb(255, 0, 0));
                                img_save.setPixel(x + w - l, y + k, qRgb(255, 0, 0));
                            }
                        }
                    }

                    img_save.save(found_unknown_error_path + file_path_to_file_name + ".jpg", "JPG", 100);

                    ++current_unknown_fault_num;
                }
            }
            else
            {
                QFile::copy(data_path + file_path_list[i], no_error_path + file_path_to_file_name + ".jpg");
            }
            qDebug() << "copy file : " << int((clock() - start)) << " ms";
            qDebug() << "total spend : " << int((clock() - start_total)) << " ms";

//            qDebug() << "process : 异常/疑似异常/背景异常/已检测/总图片数 : " << current_true_fault_num << "/" << current_unknown_fault_num << "/" << current_false_fault_num << "/" << i + 1 << "/" << file_path_list.size();
            qDebug() << "process : 异常/已检测/总图片数 : " << current_true_fault_num + current_unknown_fault_num << "/" << i + 1 << "/" << file_path_list.size();
        }

        return NoError;
    }

    //Test Zone
    if(detect_4c)
    {
        QDir dir_;

        if(!dir_.exists(catch_data_path))
        {
            dir_.mkpath(catch_data_path);
        }

        MaskDatasetController *mask_controller = new MaskDatasetController();

        mask_controller->init_param(data_path, catch_data_path, -1, -1);

        mask_controller->create_mask_work_station();

        QVector<QString> file_path_list = mask_controller->get_source_file_path();

        delete mask_controller;

        int current_img_idx = 0;

        int bigger_pixel_scale = 100;

        for(int i = 0; i < file_path_list.size(); ++i)
        {
            clock_t start = clock();
            QImage source_img(data_path + file_path_list[i]);

            qDebug() << "read image to QImage : " << int((clock() - start)) << " ms";
            start = clock();

            QImage img = source_img.convertToFormat(QImage::Format_RGB888);

            qDebug() << "convertToFormat RGB888 : " << int((clock() - start)) << " ms";
            start = clock();

            cv::Mat cv_img = darknet_detector->QImage2cvMat(img);
            qDebug() << "convert to Mat : " << int((clock() - start)) << " ms";

            // process

            start = clock();
            std::vector<bbox_t> result = darknet_detector->getDarknetResult(img);
            std::cout << int((clock() - start)) << " ms" << std::endl;

            if(result.size() > 0)
            {
                for(int j=0; j < result.size(); j++)
                {
                    int x = result[j].x;
                    int y = result[j].y;
                    int w = result[j].w;
                    int h = result[j].h;
                    int label = result[j].obj_id;

                    if(0 < label && label < 6)
                    {
                        QImage img_save = img.copy(max(0, x-bigger_pixel_scale), max(0, y-bigger_pixel_scale), min(img.width() - x + bigger_pixel_scale, w + 2 * bigger_pixel_scale), min(img.height() - y + bigger_pixel_scale, h + 2 * bigger_pixel_scale));

                        img_save.save(catch_data_path + QString::number(current_img_idx) + "_" + QString::number(j) + ".jpg", "JPG", 100);
                    }
                }

                if(dir_.exists(catch_data_path + QString::number(current_img_idx) + ".jpg"))
                {
                    dir_.remove(catch_data_path + QString::number(current_img_idx) + ".jpg");
                }

                ++current_img_idx;
            }

            qDebug() << "process : valid/scaned/total : " << current_img_idx << "/" << i + 1 << "/" << file_path_list.size();
        }

        return NoError;
    }
}
#endif

#ifdef Linux
int DeeplearningSolver::darknet_process(Connection::ConnectedData &data)
{
    float *image = new float[data.info.img_height_ * data.info.img_width_ * data.info.img_format_];

    for(int j=0; j < data.info.img_height_; j++)
    {
        for(int i=0; i < data.info.img_width_; i++)
        {
            // pixel(i,j)
            int color = data.getRed(i, j);
            if(color < 0)
            {
                color += 255;
            }
            image[j * data.info.img_width_ + i] = float(color) / 255.0;
        }
    }
    for(int j=0; j < data.info.img_height_; j++)
    {
        for(int i=0; i < data.info.img_width_; i++)
        {
            // pixel(i,j)
            int color = data.getGreen(i, j);
            if(color < 0)
            {
                color += 255;
            }
            image[data.info.img_height_ * data.info.img_width_ + j * data.info.img_width_ + i] = float(color) / 255.0;
        }
    }
    for(int j=0; j < data.info.img_height_; j++)
    {
        for(int i=0; i < data.info.img_width_; i++)
        {
            // pixel(i,j)
            int color = data.getBlue(i, j);
            if(color < 0)
            {
                color += 255;
            }
            image[2 * data.info.img_height_ * data.info.img_width_ + j * data.info.img_width_ + i] = float(color) / 255.0;
        }
    }

    // process

    clock_t start = clock();
    Mat image_in;
    darknet_detector.detect_image(image_in);
    std::vector<std::vector<double>> result = darknet_detector.get_result();
    delete(image);
    std::cout << int((clock() - start)/1000) << " ms" << std::endl;

    data.resetBBoxNum(result.size());

    for(int i=0; i < data.info.bbox_num_; i++)
    {
        data.setBBox(i, result[i][0], result[i][1], result[i][2], result[i][3], result[i][4]);
    }

    return NoError;
}

int DeeplearningSolver::darknet_test(Connection::ConnectedData &data)
{
    bool detect_2c = true;
    bool detect_4c = false;

    QString data_path = data.info.image_dir_;
    QString catch_data_path = data.info.output_dir_;

    //Test Zone
    if(detect_2c)
    {
        QDir dir_;

        float catch_fault_prob = 0.9;

        QString data_path_copy = data_path;
        catch_data_path += data_path_copy.replace(":", "").replace(".jpg", "").replace("/", "_") + "/";

        QString no_error_path = catch_data_path + "正常/正常/";//没有检测到目标 ： 正常
        QString found_true_error_path = catch_data_path + "异常/异常/";//接触网区域内检测到目标 ： 异常
        QString found_false_error_path = catch_data_path + "正常/背景异常/";//接触网区域外检测到目标 ： 背景异常
        QString found_unknown_error_path = catch_data_path + "异常/疑似异常/";//未检测到接触网区域但检测到目标 + 接触网区域内检测到目标的得分 < 0.9 ： 疑似异常

        if(!dir_.exists(catch_data_path))
        {
            dir_.mkpath(catch_data_path);
        }

        if(!dir_.exists(no_error_path))
        {
            dir_.mkpath(no_error_path);
        }
        if(!dir_.exists(found_true_error_path))
        {
            dir_.mkpath(found_true_error_path);
        }
        if(!dir_.exists(found_false_error_path))
        {
            dir_.mkpath(found_false_error_path);
        }
        if(!dir_.exists(found_unknown_error_path))
        {
            dir_.mkpath(found_unknown_error_path);
        }

        MaskDatasetController *mask_controller = new MaskDatasetController();

        mask_controller->init_param(data_path, catch_data_path, -1, -1);

        mask_controller->create_mask_work_station();

        QVector<QString> file_path_list = mask_controller->get_source_file_path();

        delete mask_controller;

        int current_true_fault_num = 0;
        int current_false_fault_num = 0;
        int current_unknown_fault_num = 0;

        //2class-bird-nest
//        int bigger_pixel_scale = 100;

        for(int i = 0; i < file_path_list.size(); ++i)
        {
            clock_t start_total = clock();
            clock_t start = clock();
            QString abs_file_path = data_path + file_path_list[i];

            QString file_path_to_file_name = abs_file_path.replace(":", "").replace(".jpg", "").replace("/", "_");

            QImage source_img(data_path + file_path_list[i]);

            qDebug() << "read image to QImage : " << int((clock() - start) / 1000) << " ms";
            start = clock();

            QImage img = source_img.convertToFormat(QImage::Format_RGB888);

            qDebug() << "convertToFormat RGB888 : " << int((clock() - start) / 1000) << " ms";
            start = clock();

            data.fromQImage(img);

            qDebug() << "fromQImage : " << int((clock() - start) / 1000) << " ms";
            start = clock();

            float *image = new float[img.width() * img.height() * 3];

            for(int j=0; j < img.height(); j++)
            {
                for(int k=0; k < img.width(); k++)
                {
                    // pixel(i,j)
                    int color = data.getRed(k, j);
                    if(color < 0)
                    {
                        color += 255;
                    }
                    image[j * img.width() + k] = float(color) / 255.0;
                }
            }
            for(int j=0; j < img.height(); j++)
            {
                for(int k=0; k < img.width(); k++)
                {
                    // pixel(i,j)
                    int color = data.getGreen(k, j);
                    if(color < 0)
                    {
                        color += 255;
                    }
                    image[img.height() * img.width() + j * img.width() + k] = float(color) / 255.0;
                }
            }
            for(int j=0; j < img.height(); j++)
            {
                for(int k=0; k < img.width(); k++)
                {
                    // pixel(i,j)
                    int color = data.getBlue(k, j);
                    if(color < 0)
                    {
                        color += 255;
                    }
                    image[2 * img.height() * img.width() + j * img.width() + k] = float(color) / 255.0;
                }
            }

            // process

            start = clock();
            std::vector<std::pair<char *, std::vector<float>>> region_result;// = darknet_detector.getRegionResult(image, img.width(), img.height(), 3);

            qDebug() << "getRegionResult : " << int((clock() - start) / 1000) << " ms";
            start = clock();

            std::vector<std::pair<char *, std::vector<float>>> component_result;// = darknet_detector.getComponentResult(image, img.width(), img.height(), 3);

            delete(image);

            qDebug() << "getComponentResult : " << int((clock() - start) / 1000) << " ms";
            start = clock();

            std::vector<std::pair<char *, std::vector<float>>> result;
            std::vector<std::pair<char *, std::vector<float>>> false_result;
            std::vector<std::pair<char *, std::vector<float>>> unknown_result;

            if(component_result.size() > 0)
            {
                for(std::pair<char *, std::vector<float>> component_bbox : component_result)
                {
                    if(region_result.size() > 0)
                    {
                        bool is_in_region = false;

                        for(std::pair<char *, std::vector<float>> region_bbox : region_result)
                        {
                            if(component_bbox.second[1] + component_bbox.second[3] < region_bbox.second[1] ||
                                    component_bbox.second[1] > region_bbox.second[1] + region_bbox.second[3] ||
                                    component_bbox.second[2] + component_bbox.second[4] < region_bbox.second[2] ||
                                    component_bbox.second[2] > region_bbox.second[2] + region_bbox.second[4])
                            {
                                continue;
                            }
                            else
                            {
                                is_in_region = true;
                                break;
                            }
                        }

                        if(is_in_region)
                        {
                            result.push_back(component_bbox);
                        }
                        else
                        {
                            false_result.push_back(component_bbox);
                        }
                    }
                    else
                    {
                        unknown_result.push_back(component_bbox);
                    }
                }
                qDebug() << "compute to get true fault bbox : " << int((clock() - start) / 1000) << " ms";
            }

            start = clock();
            int line_width = 3;

            if(result.size() > 0 || false_result.size() > 0 || unknown_result.size() > 0)
            {
                if(result.size() > 0)
                {
                    QImage img_save = img.copy();

//                    QPainter painter(&img_save);

//                    painter.setCompositionMode(QPainter::CompositionMode_SourceIn);

//                    QPen pen = painter.pen();
//                    pen.setColor(Qt::red);
//                    QFont font = painter.font();
//                    font.setBold(true);
//                    font.setPixelSize(30);
//                    painter.setPen(pen);
//                    painter.setFont(font);

                    bool have_high_prob = false;

                    for(int j=0; j < result.size(); j++)
                    {
                        int x = result[j].second[1];
                        int y = result[j].second[2];
                        int w = result[j].second[3];
                        int h = result[j].second[4];
//                        int label = result[j].second[5];
//                        float prob = result[j].prob;
                        float prob = 1.0;

                        if(!have_high_prob)
                        {
                            if(prob >= catch_fault_prob)
                            {
                                have_high_prob = true;
                            }
                        }

//                        painter.drawText(x, y, "");

                        for(int k = 0; k < w; ++k)
                        {
                            for(int l = 0; l <= line_width; ++l)
                            {
                                img_save.setPixel(x + k, y + l, qRgb(255, 0, 0));
                                img_save.setPixel(x + k, y - l, qRgb(255, 0, 0));

                                img_save.setPixel(x + k, y + h + l, qRgb(255, 0, 0));
                                img_save.setPixel(x + k, y + h - l, qRgb(255, 0, 0));
                            }
                        }

                        for(int k = 0; k < h; ++k)
                        {
                            for(int l = 0; l <= line_width; ++l)
                            {
                                img_save.setPixel(x + l, y + k, qRgb(255, 0, 0));
                                img_save.setPixel(x - l, y + k, qRgb(255, 0, 0));

                                img_save.setPixel(x + w + l, y + k, qRgb(255, 0, 0));
                                img_save.setPixel(x + w - l, y + k, qRgb(255, 0, 0));
                            }
                        }
                    }

                    if(have_high_prob)
                    {
                        img_save.save(found_true_error_path + file_path_to_file_name + ".jpg", "JPG", 100);
                    }
                    else
                    {
                        img_save.save(found_unknown_error_path + file_path_to_file_name + ".jpg", "JPG", 100);
                    }

                    ++current_true_fault_num;
                }
                if(false_result.size() > 0)
                {
                    QImage img_save = img.copy();

                    for(int j=0; j < false_result.size(); j++)
                    {
                        int x = false_result[j].second[1];
                        int y = false_result[j].second[2];
                        int w = false_result[j].second[3];
                        int h = false_result[j].second[4];
//                        int label = false_result[j].second[5];

                        for(int k = 0; k < w; ++k)
                        {
                            for(int l = 0; l <= line_width; ++l)
                            {
                                img_save.setPixel(x + k, y + l, qRgb(255, 0, 0));
                                img_save.setPixel(x + k, y - l, qRgb(255, 0, 0));

                                img_save.setPixel(x + k, y + h + l, qRgb(255, 0, 0));
                                img_save.setPixel(x + k, y + h - l, qRgb(255, 0, 0));
                            }
                        }

                        for(int k = 0; k < h; ++k)
                        {
                            for(int l = 0; l <= line_width; ++l)
                            {
                                img_save.setPixel(x + l, y + k, qRgb(255, 0, 0));
                                img_save.setPixel(x - l, y + k, qRgb(255, 0, 0));

                                img_save.setPixel(x + w + l, y + k, qRgb(255, 0, 0));
                                img_save.setPixel(x + w - l, y + k, qRgb(255, 0, 0));
                            }
                        }
                    }

                    img_save.save(found_false_error_path + file_path_to_file_name + ".jpg", "JPG", 100);

                    ++current_false_fault_num;
                }
                if(unknown_result.size() > 0)
                {
                    QImage img_save = img.copy();

                    for(int j=0; j < unknown_result.size(); j++)
                    {
                        int x = unknown_result[j].second[1];
                        int y = unknown_result[j].second[2];
                        int w = unknown_result[j].second[3];
                        int h = unknown_result[j].second[4];
//                        int label = unknown_result[j].second[5];

                        for(int k = 0; k < w; ++k)
                        {
                            for(int l = 0; l <= line_width; ++l)
                            {
                                img_save.setPixel(x + k, y + l, qRgb(255, 0, 0));
                                img_save.setPixel(x + k, y - l, qRgb(255, 0, 0));

                                img_save.setPixel(x + k, y + h + l, qRgb(255, 0, 0));
                                img_save.setPixel(x + k, y + h - l, qRgb(255, 0, 0));
                            }
                        }

                        for(int k = 0; k < h; ++k)
                        {
                            for(int l = 0; l <= line_width; ++l)
                            {
                                img_save.setPixel(x + l, y + k, qRgb(255, 0, 0));
                                img_save.setPixel(x - l, y + k, qRgb(255, 0, 0));

                                img_save.setPixel(x + w + l, y + k, qRgb(255, 0, 0));
                                img_save.setPixel(x + w - l, y + k, qRgb(255, 0, 0));
                            }
                        }
                    }

                    img_save.save(found_unknown_error_path + file_path_to_file_name + ".jpg", "JPG", 100);

                    ++current_unknown_fault_num;
                }
            }
            else
            {
                QFile::copy(data_path + file_path_list[i], no_error_path + file_path_to_file_name + ".jpg");
            }
            qDebug() << "copy file : " << int((clock() - start) / 1000) << " ms";
            qDebug() << "total spend : " << int((clock() - start_total) / 1000) << " ms";

//            qDebug() << "process : 异常/疑似异常/背景异常/已检测/总图片数 : " << current_true_fault_num << "/" << current_unknown_fault_num << "/" << current_false_fault_num << "/" << i + 1 << "/" << file_path_list.size();
            qDebug() << "process : 异常/已检测/总图片数 : " << current_true_fault_num + current_unknown_fault_num << "/" << i + 1 << "/" << file_path_list.size();
        }

        return NoError;
    }

    //Test Zone
    if(detect_4c)
    {
        QDir dir_;

        if(!dir_.exists(catch_data_path))
        {
            dir_.mkpath(catch_data_path);
        }

        MaskDatasetController *mask_controller = new MaskDatasetController();

        mask_controller->init_param(data_path, catch_data_path, -1, -1);

        mask_controller->create_mask_work_station();

        QVector<QString> file_path_list = mask_controller->get_source_file_path();

        delete mask_controller;

        int current_img_idx = 0;

        int bigger_pixel_scale = 100;

        for(int i = 0; i < file_path_list.size(); ++i)
        {
            clock_t start_total = clock();
            clock_t start = clock();
            QString abs_file_path = data_path + file_path_list[i];

            QString file_path_to_file_name = abs_file_path.replace(":", "").replace(".jpg", "").replace("/", "_");

            QImage source_img(data_path + file_path_list[i]);

            qDebug() << "read image to QImage : " << int((clock() - start) / 1000) << " ms";
            start = clock();

            QImage img = source_img.convertToFormat(QImage::Format_RGB888);

            qDebug() << "convertToFormat RGB888 : " << int((clock() - start) / 1000) << " ms";
            start = clock();

            data.fromQImage(img);

            qDebug() << "fromQImage : " << int((clock() - start) / 1000) << " ms";
            start = clock();

            float *image = new float[img.width() * img.height() * 3];

            for(int j=0; j < img.height(); j++)
            {
                for(int k=0; k < img.width(); k++)
                {
                    // pixel(i,j)
                    int color = data.getRed(k, j);
                    if(color < 0)
                    {
                        color += 255;
                    }
                    image[j * img.width() + k] = float(color) / 255.0;
                }
            }
            for(int j=0; j < img.height(); j++)
            {
                for(int k=0; k < img.width(); k++)
                {
                    // pixel(i,j)
                    int color = data.getGreen(k, j);
                    if(color < 0)
                    {
                        color += 255;
                    }
                    image[img.height() * img.width() + j * img.width() + k] = float(color) / 255.0;
                }
            }
            for(int j=0; j < img.height(); j++)
            {
                for(int k=0; k < img.width(); k++)
                {
                    // pixel(i,j)
                    int color = data.getBlue(k, j);
                    if(color < 0)
                    {
                        color += 255;
                    }
                    image[2 * img.height() * img.width() + j * img.width() + k] = float(color) / 255.0;
                }
            }

            // process

            start = clock();
            std::vector<std::pair<char *, std::vector<float>>> result;// = darknet_detector.getDarknetResult(image, img.width(), img.height(), 3);

            qDebug() << "getDarknetResult : " << int((clock() - start) / 1000) << " ms";

            if(result.size() > 0)
            {
                for(int j=0; j < result.size(); j++)
                {
                    int x = result[j].second[1];
                    int y = result[j].second[2];
                    int w = result[j].second[3];
                    int h = result[j].second[4];
                    int label = result[j].second[5];

                    if(0 < label && label < 6)
                    {
                        QImage img_save = img.copy(max(0, x-bigger_pixel_scale), max(0, y-bigger_pixel_scale), min(img.width() - x + bigger_pixel_scale, w + 2 * bigger_pixel_scale), min(img.height() - y + bigger_pixel_scale, h + 2 * bigger_pixel_scale));

                        img_save.save(catch_data_path + QString::number(current_img_idx) + "_" + QString::number(j) + ".jpg", "JPG", 100);
                    }
                }

                if(dir_.exists(catch_data_path + QString::number(current_img_idx) + ".jpg"))
                {
                    dir_.remove(catch_data_path + QString::number(current_img_idx) + ".jpg");
                }

                ++current_img_idx;
            }

            qDebug() << "process : valid/scaned/total : " << current_img_idx << "/" << i + 1 << "/" << file_path_list.size();
        }

        return NoError;
    }
}
#endif

int DeeplearningSolver::lapnet_train_process(Connection::ConnectedData &data)
{
//    QString srcPath = QDir::currentPath() + "/../Server_DataBase/received_dataset/lapnet_dataset";
//    QString dstPath = QDir::currentPath() + "/../src/Python/LapNet/Dataset/train_dataset";

//    if(!Connection::cpDir(srcPath, dstPath))
//    {
//        qDebug() << "copy dir failed !" << endl;
//        return Error;
//    }

    lapnet_train_solver->train(data.info.conda_env_name_);

    return NoError;
}

int DeeplearningSolver::lapnet_stoptrain_process(Connection::ConnectedData &data)
{
    QFile file("./../src/Python/LapNet/temp_train_mode.txt");

    if(!file.open(QFile::WriteOnly))
    {
        qDebug() << "Failed to open lapnet train label !" << endl;
    }

    file.resize(0);

    file.write("False");

    file.close();

    return NoError;
}

int DeeplearningSolver::lapnet_file_process(Connection::ConnectedData &data)
{
    QString string_file_type;

    if(data.info.file_type_ == Connection::File_OBJ)
    {
        string_file_type = "obj";
    }
    else if(data.info.file_type_ == Connection::File_Dataset)
    {
        string_file_type = "dataset";
    }

    if(data.info.file_name_.contains('/'))
    {
        QString folder_path = QDir::currentPath() + "/../Server_DataBase/received_" + string_file_type + "/lapnet_" + string_file_type + "/" + data.info.file_name_.left(data.info.file_name_.lastIndexOf('/'));

        QDir dir(folder_path);

        if(!dir.exists())
        {
            dir.mkpath(folder_path);
        }
    }

    QFile file_recieved(QDir::currentPath() + "/../Server_DataBase/received_" + string_file_type + "/lapnet_" + string_file_type + "/" + data.info.file_name_);

    if(!file_recieved.open(QFile::WriteOnly))
    {
        qDebug() << "Couldn't write---1" << endl;
    }

    data.toQFile(file_recieved);

    file_recieved.close();

    return NoError;
}

int DeeplearningSolver::lapnet_getresult_process(Connection::ConnectedData &data)
{
    //文件夹路径
    QString modelDirPath = QFileInfo(QDir::currentPath() + "/../src/Python/LapNet/trained_model").absoluteFilePath();

    if (modelDirPath.isEmpty())
    {
        return NoError;
    }
    else
    {
        qDebug() << "modelDirPath=" << modelDirPath;
    }

    QString base_model_name = "LapNet_chkpt_better_epoch";

    int max_model_epoch = 0;
    int max_model_epoch_idx = 0;

    QFileInfoList file_list = Connection::GetFileList(modelDirPath);

    for(int i = 0; i < file_list.size(); ++i)
    {
        int current_model_epoch = file_list[i].absoluteFilePath().split(base_model_name)[1].split("_")[0].toInt();

        if(current_model_epoch > max_model_epoch)
        {
            max_model_epoch = current_model_epoch;
            max_model_epoch_idx = i;
        }
    }

    QStringList split_list = modelDirPath.split("/");

    QString base_path = split_list[split_list.size() - 1];

//                    for(int i = 0; i < file_list.size(); ++i)
//                    {
//                        QString filename = file_list[i].absoluteFilePath();

//                        QFile outfile(filename);

//                        qDebug() << filename << endl;

//                        filename = base_path + file_list[i].absoluteFilePath().split(modelDirPath)[1];

//                        qDebug() << filename << endl;

//                        if(!outfile.open(QFile::ReadOnly))
//                        {
//                            std::cout << "打开文件失败！" << std::endl;

//                            return;
//                        }

//                        data_main_.fromQFile(outfile);

//                        this->write(toFileHeader(data_main_.info.total_size_, filename, Model_LapNet) + data_main_.data_);

//                        this->waitForBytesWritten();
//                    }

    QString filename = file_list[max_model_epoch_idx].absoluteFilePath();

    QFile outfile(filename);

    qDebug() << filename << endl;

    filename = base_path + file_list[max_model_epoch_idx].absoluteFilePath().split(modelDirPath)[1];

    qDebug() << filename << endl;

    if(!outfile.open(QFile::ReadOnly))
    {
        qDebug() << "打开文件失败！" << endl;

        return Error;
    }

    data.info.file_name_ = filename;

    data.fromQFile(outfile);

    return NoError;
}

int DeeplearningSolver::darknet_createdata_process(Connection::ConnectedData &data)
{
    data_rebuild_solver->Create_Dataset(data.info.create_data_num_, data.info.max_obj_num_per_img_);

    QString srcPath = QDir::currentPath() + "/../Server_DataBase/create_dataset/darknet_dataset";
    QString dstPath = QDir::currentPath() + "/../Server_DataBase/train_dataset/darknet_dataset/sources";

    QDir dir(dstPath);

    if(dir.exists())
    {
        dir.removeRecursively();
    }

    if(!Connection::cpDir(srcPath, dstPath))
    {
        qDebug() << "copy dir failed !" << endl;
    }

    return NoError;
}

int DeeplearningSolver::darknet_enhancement_process(Connection::ConnectedData &data)
{
    data_rebuild_solver->sample_enhancement(data.info.conda_env_name_, data.info.enhancement_factor_);

    return NoError;
}

int DeeplearningSolver::darknet_datachange_process(Connection::ConnectedData &data)
{
    QFile file(QDir::currentPath() + "/../Server_DataBase/train_dataset/darknet_dataset/my_labels.txt");

    if(file.open(QFile::WriteOnly))
    {
        file.resize(0);

        for(int i = 0; i < data.info.label_num_; ++i)
        {
            file.write((QString::number(i) + "\n").toStdString().c_str());
        }

        file.close();
    }
    else
    {
        qDebug() << "DeeplearningSolver::darknet_datachange_process : open my_labels.txt failed.";
        return NoError;
    }

    file.setFileName(QDir::currentPath() + "/../Server_DataBase/train_dataset/darknet_dataset/train_percent.txt");

    if(file.open(QFile::WriteOnly))
    {
        file.resize(0);

        file.write((QString::number(data.info.train_percent_)).toStdString().c_str());

        file.close();
    }
    else
    {
        qDebug() << "DeeplearningSolver::darknet_datachange_process : open train_percent.txt failed.";
        return NoError;
    }

    data_rebuild_solver->datachange(data.info.conda_env_name_);

    return NoError;
}

int DeeplearningSolver::darknet_train_process(Connection::ConnectedData &data)
{
//    data_rebuild_solver->Create_Dataset();

//    QString srcPath = QDir::currentPath() + "/../Server_DataBase/create_dataset/darknet_dataset";
//    QString dstPath = QDir::currentPath() + "/../Server_DataBase/train_dataset/darknet_dataset/sources";

//    QDir dir(dstPath);

//    if(dir.exists())
//    {
//        dir.removeRecursively();
//    }

//    if(!Connection::cpDir(srcPath, dstPath))
//    {
//        qDebug() << "copy dir failed !" << endl;
//    }

////    data_rebuild_solver->sample_enhancement(data.info.conda_env_name_);
////    data_rebuild_solver->datachange(data.info.conda_env_name_);

////    QFile file("../Server_DataBase/train_dataset/darknet_dataset/cfg/coco.cfg");

////    if(!file.open(QIODevice::WriteOnly))
////    {
////        qDebug() << "can not open coco.cfg" << endl;
////    }
////    else
////    {
////        file.resize(0);

////        file.write(("classes= " + QString::number(data_rebuild_solver->class_num) + "\n").toStdString().c_str());
////        file.write(("train  = " + QDir::currentPath() + "/../Server_DataBase/train_dataset/darknet_dataset/Main/train.txt\n").toStdString().c_str());
////        file.write("valid  = coco_testdev\n");
////        file.write(("names  = " + QDir::currentPath() + "/../Server_DataBase/train_dataset/darknet_dataset/cfg/coco.names\n").toStdString().c_str());
////        file.write(("backup = " + QDir::currentPath() + "/../darknet-master/backup\n").toStdString().c_str());
////        file.write("eval=coco\n");

////        file.close();
////    }

    if(data.info.message_type_ == Connection::Type_Train)
    {
        data_rebuild_solver->train(data.info.conda_env_name_, QString("train"));
    }
    else if(data.info.message_type_ == Connection::Type_Valid)
    {
        data_rebuild_solver->train(data.info.conda_env_name_, QString("valid"));
    }

    return NoError;
}

int DeeplearningSolver::darknet_stoptrain_process(Connection::ConnectedData &data)
{
    QFile file(QDir::currentPath() + "/../src/Python/LapNet/temp_train_mode.txt");

    if(!file.open(QFile::WriteOnly))
    {
        qDebug() << "Failed to open lapnet train label !" << endl;
    }

    file.resize(0);

    file.write("False");

    file.close();

    return NoError;
}

int DeeplearningSolver::darknet_file_process(Connection::ConnectedData &data)
{
    QString string_file_type;

    if(data.info.file_type_ == Connection::File_OBJ)
    {
        string_file_type = "obj";
    }
    else if(data.info.file_type_ == Connection::File_Dataset)
    {
        string_file_type = "dataset";
    }

    if(data.info.file_name_.contains('/'))
    {
        QString folder_path = QDir::currentPath() + "/../Server_DataBase/received_" + string_file_type + "/darknet_" + string_file_type + "/" + data.info.file_name_.left(data.info.file_name_.lastIndexOf('/'));

        QDir dir(folder_path);

        if(!dir.exists())
        {
            dir.mkpath(folder_path);
        }
    }

    QFile file_recieved(QDir::currentPath() + "/../Server_DataBase/received_" + string_file_type + "/darknet_" + string_file_type + "/" + data.info.file_name_);

    if(!file_recieved.open(QFile::WriteOnly))
    {
        qDebug() << "Couldn't write---1" << endl;
    }

    data.toQFile(file_recieved);

    file_recieved.close();

    return NoError;
}

int DeeplearningSolver::darknet_getresult_process(Connection::ConnectedData &data)
{
    //文件夹路径
    QString modelDirPath = QFileInfo(QDir::currentPath() + "/../Linux/darknet-master/backup").absoluteFilePath();

    if (modelDirPath.isEmpty())
    {
        return NoError;
    }
    else
    {
        qDebug() << "modelDirPath=" << modelDirPath;
    }

    QString base_model_name = "yolov3_train_";

    int max_model_epoch = 0;
    int max_model_epoch_idx = 0;

    QFileInfoList file_list = Connection::GetFileList(modelDirPath);

    for(int i = 0; i < file_list.size(); ++i)
    {
        int current_model_epoch = file_list[i].absoluteFilePath().split(base_model_name)[1].split("_")[0].toInt();

        if(current_model_epoch > max_model_epoch)
        {
            max_model_epoch = current_model_epoch;
            max_model_epoch_idx = i;
        }
    }

    QStringList split_list = modelDirPath.split("/");

    QString base_path = split_list[split_list.size() - 1];

//                    for(int i = 0; i < file_list.size(); ++i)
//                    {
//                        QString filename = file_list[i].absoluteFilePath();

//                        QFile outfile(filename);

//                        qDebug() << filename << endl;

//                        filename = base_path + file_list[i].absoluteFilePath().split(modelDirPath)[1];

//                        qDebug() << filename << endl;

//                        if(!outfile.open(QFile::ReadOnly))
//                        {
//                            std::cout << "打开文件失败！" << std::endl;

//                            return;
//                        }

//                        data_main_.fromQFile(outfile);

//                        this->write(toFileHeader(data_main_.info.total_size_, filename, Model_LapNet) + data_main_.data_);

//                        this->waitForBytesWritten();
//                    }

    QString filename = file_list[max_model_epoch_idx].absoluteFilePath();

    QFile outfile(filename);

    qDebug() << filename << endl;

    filename = base_path + file_list[max_model_epoch_idx].absoluteFilePath().split(modelDirPath)[1];

    qDebug() << filename << endl;

    if(!outfile.open(QFile::ReadOnly))
    {
        qDebug() << "打开文件失败！" << endl;

        return Error;
    }

    data.info.file_name_ = filename;

    data.fromQFile(outfile);

    return NoError;
}

int DeeplearningSolver::darknet_video_process(Connection::ConnectedData &data)
{
    if(!video_cap_start)
    {
        video_cap_manager.set_param("192.168.0.203", "8085", 0, 300, "../Cap_Video");

        video_cap_start = video_cap_manager.connect_camera();
    }

    if(video_cap_start)
    {
        camera_img = video_cap_manager.receive_mjpg();

        data.fromQImage(camera_img);

        return NoError;
    }

    data.info.message_type_ = Connection::Type_StopVideo;

    return Error;
}

int DeeplearningSolver::darknet_stopvideo_process(Connection::ConnectedData &data)
{
    video_cap_start = false;

    return NoError;
}

int DeeplearningSolver::update_received_file_message_process(Connection::ConnectedData &data)
{
    received_files_namager.update_files_message();

    QFile file_sent(QDir::currentPath() + "/../Server_DataBase/message_set/received_files_list.txt");

    if(!file_sent.open(QFile::ReadOnly))
    {
        qDebug() << "Couldn't read---1" << endl;
    }

    data.fromQFile(file_sent);

    file_sent.close();

    return NoError;
}

int DeeplearningSolver::init_model_process(Connection::ConnectedData &data)
{
    init_model(data.info.conda_env_name_);

    return NoError;
}

DeeplearningFilter::DeeplearningFilter(std::shared_ptr<DeeplearningSolver> solver):solver_(solver)
{

}

int DeeplearningFilter::process(Connection::ConnectedData &data)
{
    if(solver_.get())
    {
        if(data.info.model_type_ == Connection::Model_LapNet)
        {
            if(data.info.message_type_ == Connection::Type_Image)
            {
                return solver_->lapnet_process(data);
            }
            else if(data.info.message_type_ == Connection::Type_ImageList)
            {
                return solver_->lapnet_process(data);
            }
            else if(data.info.message_type_ == Connection::Type_Video)
            {
                return solver_->lapnet_process(data);
            }
            else if(data.info.message_type_ == Connection::Type_Train)
            {
                return solver_->lapnet_train_process(data);
            }
            else if(data.info.message_type_ == Connection::Type_StopTrain)
            {
                return solver_->lapnet_stoptrain_process(data);
            }
            else if(data.info.message_type_ == Connection::Type_Valid)
            {
                return solver_->lapnet_train_process(data);
            }
            else if(data.info.message_type_ == Connection::Type_File)
            {
                return solver_->lapnet_file_process(data);
            }
            else if(data.info.message_type_ == Connection::Type_ReturnResult)
            {
                return solver_->lapnet_getresult_process(data);
            }
        }
        else if(data.info.model_type_ == Connection::Model_Darknet)
        {
            if(data.info.message_type_ == Connection::Type_Image)
            {
                return solver_->darknet_process(data);
            }
            else if(data.info.message_type_ == Connection::Type_ImageList)
            {
                return solver_->darknet_process(data);
            }
            else if(data.info.message_type_ == Connection::Type_Frame)
            {
                return solver_->darknet_video_process(data);
            }
            else if(data.info.message_type_ == Connection::Type_FrameBBox)
            {
                data.fromQImage(solver_->camera_img);
                return solver_->darknet_process(data);
            }
            else if(data.info.message_type_ == Connection::Type_Video)
            {
                return solver_->darknet_video_process(data);
            }
            else if(data.info.message_type_ == Connection::Type_VideoBBox)
            {
                data.fromQImage(solver_->camera_img);
                return solver_->darknet_process(data);
            }
            else if(data.info.message_type_ == Connection::Type_StopVideo)
            {
                return solver_->darknet_stopvideo_process(data);
            }
            else if(data.info.message_type_ == Connection::Type_CreateData)
            {
                return solver_->darknet_createdata_process(data);
            }
            else if(data.info.message_type_ == Connection::Type_Enhancement)
            {
                return solver_->darknet_enhancement_process(data);
            }
            else if(data.info.message_type_ == Connection::Type_DataChange)
            {
                return solver_->darknet_datachange_process(data);
            }
            else if(data.info.message_type_ == Connection::Type_Train)
            {
                return solver_->darknet_train_process(data);
            }
            else if(data.info.message_type_ == Connection::Type_StopTrain)
            {
                return solver_->darknet_stoptrain_process(data);
            }
            else if(data.info.message_type_ == Connection::Type_Valid)
            {
                return solver_->darknet_train_process(data);
            }
            else if(data.info.message_type_ == Connection::Type_File)
            {
                return solver_->darknet_file_process(data);
            }
            else if(data.info.message_type_ == Connection::Type_Test)
            {
                return solver_->darknet_test(data);
            }
        }
        else if(data.info.model_type_ == Connection::Model_Free)
        {
            if(data.info.message_type_ == Connection::Type_ReceivedFileMsg)
            {
                return solver_->update_received_file_message_process(data);
            }
            else if(data.info.message_type_ == Connection::Type_InitModel)
            {
                solver_->close_server();
                return solver_->init_model_process(data);
            }
            else if(data.info.message_type_ == Connection::Type_ReturnResult)
            {
                return solver_->darknet_getresult_process(data);
            }
        }
    }
    return DeeplearningSolver::NoError;
}

}
