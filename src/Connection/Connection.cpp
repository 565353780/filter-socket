#include "Connection.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
#include <QImage>
#include <iostream>
#include <sstream>
#if 1
std::stringstream sstream;
#define LogOut sstream
#define LogClean sstream.str("");
#else
#define LogOut std::cout
#define LogClean
#endif


namespace FilterSocket {


Connection::Connection(QObject *parent):QTcpSocket(parent)
{

}

void Connection::setFilter(std::shared_ptr<FilterInterface> filter)
{
    filter_ = filter;
}

void Connection::processData()
{
    LogOut<<"processData"<<"\n";

    if(!this->isValid())
    {
        this->waitForReadyRead();
    }

    if(state_ == State_WaitConnection || state_ == State_ReadHeader)
    {
        header_array_.append(this->readAll());

        QByteArray header_end = messageFlag("HeaderEnd");

        int header_end_idx = header_array_.indexOf(header_end);

        if(header_end_idx >= 0)
        {
            state_ = State_ReadData;

            QByteArray darr = header_array_.left(header_end_idx);
            QJsonDocument doc;
            LogOut<<darr.toStdString()<<"\n";
            doc = QJsonDocument::fromJson(darr);
            QJsonObject obj = doc.object();

            if(obj.contains("msg_type"))
            {
                data_main_.info.message_type_ = obj.value("msg_type").toInt();
            }
            if(obj.contains("model_type"))
            {
                data_main_.info.model_type_ = obj.value("model_type").toInt();
            }
            if(obj.contains("total_size"))
            {
                data_main_.info.total_size_ = obj.value("total_size").toInt();
            }
            if(obj.contains("img_width"))
            {
                data_main_.info.img_width_ = obj.value("img_width").toInt();
            }
            if(obj.contains("img_height"))
            {
                data_main_.info.img_height_ = obj.value("img_height").toInt();
            }
            if(obj.contains("img_format"))
            {
                data_main_.info.img_format_ = obj.value("img_format").toInt();
            }
            if(obj.contains("conda_env_name"))
            {
                data_main_.info.conda_env_name_ = QString(obj.value("conda_env_name").toString());
            }
            if(obj.contains("bbox_num"))
            {
                data_main_.info.bbox_num_ = obj.value("bbox_num").toInt();
            }
            if(obj.contains("file_name"))
            {
                data_main_.info.file_name_ = QString(obj.value("file_name").toString());
            }
            if(obj.contains("file_type"))
            {
                data_main_.info.file_type_ = obj.value("file_type").toInt();
            }
            if(obj.contains("create_data_num"))
            {
                data_main_.info.create_data_num_ = obj.value("create_data_num").toInt();
            }
            if(obj.contains("max_obj_num_per_img"))
            {
                data_main_.info.max_obj_num_per_img_ = obj.value("max_obj_num_per_img").toInt();
            }
            if(obj.contains("enhancement_factor"))
            {
                data_main_.info.enhancement_factor_ = obj.value("enhancement_factor").toInt();
            }
            if(obj.contains("train_percent"))
            {
                data_main_.info.train_percent_ = obj.value("train_percent").toInt();
            }
            if(obj.contains("label_num"))
            {
                data_main_.info.label_num_ = obj.value("label_num").toInt();
            }
            if(obj.contains("image_dir"))
            {
                data_main_.info.image_dir_ = obj.value("image_dir").toString();
            }
            if(obj.contains("output_dir"))
            {
                data_main_.info.output_dir_ = obj.value("output_dir").toString();
            }

            data_main_.data_ = header_array_.right(header_array_.length() - (header_end_idx + header_end.length()));
            LogOut<<"DataSize: "<<data_main_.data_.size()<<"\n";
            header_array_ = QByteArray();
        }
    }
    if(state_ == State_ReadData)
    {
        LogOut<<"ReadData"<<"\n";

        data_main_.data_.append( this->readAll());
        if(data_main_.data_.size() >= data_main_.info.total_size_)
        {
            state_ = State_Finished;
        }
    }
    if(state_ == State_Finished)
    {
        LogOut<<"Finished"<<"\n";

        if(filter_.get() && !is_client_)
        {
            filter_->process(data_main_);
            LogOut<<"Finished---0"<<"\n";

            QJsonObject header_Obj;

//            switch (data_main_.info.model_type_)
//            {
//            case Model_LapNet:

//                switch (data_main_.info.message_type_)
//                {
//                case Type_Image:

//                    this->write(toImageHeader(data_main_.info.img_width_,data_main_.info.img_height_,data_main_.info.img_format_, Model_LapNet) + data_main_.data_);
//                    LogOut<<"Finished write---1"<<"\n";

//                    break;
//                case Type_ImageList:

//                    this->write(toImageListHeader(data_main_.info.img_width_,data_main_.info.img_height_,data_main_.info.img_format_, Model_LapNet) + data_main_.data_);
//                    LogOut<<"Finished write---1"<<"\n";

//                    break;
//                case Type_Video:

//                    this->write(toImageHeader(data_main_.info.img_width_,data_main_.info.img_height_,data_main_.info.img_format_, Model_LapNet) + data_main_.data_);
//                    LogOut<<"Finished write---1"<<"\n";

//                    break;
//                case Type_Train:

//                    qDebug() << "LapNet Train Msg !" << endl;

//                    break;
//                case Type_File:

//                    QString string_file_type;

//                    if(data_main_.info.file_type_ == File_OBJ)
//                    {
//                        string_file_type = "obj";
//                    }
//                    else if(data_main_.info.file_type_ == File_Dataset)
//                    {
//                        string_file_type = "dataset";
//                    }

//                    if(data_main_.info.file_name_.contains('/'))
//                    {
//                        QString folder_path = QDir::currentPath() + "/../Server_DataBase/received_" + string_file_type + "/lapnet_" + string_file_type + "/" + data_main_.info.file_name_.left(data_main_.info.file_name_.lastIndexOf('/'));

//                        QDir dir(folder_path);

//                        if(!dir.exists())
//                        {
//                            dir.mkpath(folder_path);
//                        }
//                    }

//                    QFile file_recieved(QDir::currentPath() + "/../Server_DataBase/received_" + string_file_type + "/lapnet_" + string_file_type + "/" + data_main_.info.file_name_);

//                    if(!file_recieved.open(QFile::WriteOnly))
//                    {
//                        LogOut<<"Couldn't write---1"<<"\n";
//                    }

//                    data_main_.toQFile(file_recieved);

//                    file_recieved.close();

//                    data_main_.clear();

//                    this->write(toFileFinishedHeader(Model_LapNet) + data_main_.data_);

//                    LogOut<<"Finished recieve---1"<<"\n";

//                    break;
//                case Type_ReturnResult:

//                    //文件夹路径
//                    QString modelDirPath = QFileInfo(QDir::currentPath() + "/../src/Python/LapNet/trained_model").absoluteFilePath();

//                    if (modelDirPath.isEmpty())
//                    {
//                        return;
//                    }
//                    else
//                    {
//                        qDebug() << "modelDirPath=" << modelDirPath;
//                    }

//                    QString base_model_name = "LapNet_chkpt_better_epoch";

//                    int max_model_epoch = 0;
//                    int max_model_epoch_idx = 0;

//                    QFileInfoList file_list = GetFileList(modelDirPath);

//                    for(int i = 0; i < file_list.size(); ++i)
//                    {
//                        int current_model_epoch = file_list[i].absoluteFilePath().split(base_model_name)[1].split("_")[0].toInt();

//                        if(current_model_epoch > max_model_epoch)
//                        {
//                            max_model_epoch = current_model_epoch;
//                            max_model_epoch_idx = i;
//                        }
//                    }

//                    QStringList split_list = modelDirPath.split("/");

//                    QString base_path = split_list[split_list.size() - 1];

////                    for(int i = 0; i < file_list.size(); ++i)
////                    {
////                        QString filename = file_list[i].absoluteFilePath();

////                        QFile outfile(filename);

////                        qDebug() << filename << endl;

////                        filename = base_path + file_list[i].absoluteFilePath().split(modelDirPath)[1];

////                        qDebug() << filename << endl;

////                        if(!outfile.open(QFile::ReadOnly))
////                        {
////                            std::cout << "打开文件失败！" << std::endl;

////                            return;
////                        }

////                        data_main_.info.fromQFile(outfile);

////                        this->write(toFileHeader(data_main_.info.total_size_, filename, Model_LapNet) + data_main_.data_);

////                        this->waitForBytesWritten();
////                    }

//                    QString filename = file_list[max_model_epoch_idx].absoluteFilePath();

//                    QFile outfile(filename);

//                    qDebug() << filename << endl;

//                    filename = base_path + file_list[max_model_epoch_idx].absoluteFilePath().split(modelDirPath)[1];

//                    qDebug() << filename << endl;

//                    if(!outfile.open(QFile::ReadOnly))
//                    {
//                        std::cout << "打开文件失败！" << std::endl;

//                        return;
//                    }

//                    data_main_.fromQFile(outfile);

//                    this->write(toFileHeader(data_main_.info.total_size_, filename, Model_LapNet) + data_main_.data_);

//                    this->waitForBytesWritten();

//                    break;
//                }
//                break;
//            case Model_Yolov3:
//                break;
//            case Model_Darknet:
//                break;
//            case Model_Free:
//                break;
//            default:
//                break;
//            }

            if(data_main_.info.model_type_ == Model_LapNet)
            {
                if(data_main_.info.message_type_ == Type_Image)
                {
                    header_Obj["msg_type"] = Type_Image;
                    header_Obj["img_width"] = data_main_.info.img_width_;
                    header_Obj["img_height"] = data_main_.info.img_height_;
                    header_Obj["img_format"] = data_main_.info.img_format_;
                    header_Obj["total_size"] = data_main_.info.img_width_ * data_main_.info.img_height_ * 3;
                    header_Obj["model_type"] = Model_LapNet;

                    this->write(toHeader(header_Obj) + data_main_.data_);
                    LogOut<<"Finished write---1"<<"\n";
                }
                else if(data_main_.info.message_type_ == Type_ImageList)
                {
                    header_Obj["msg_type"] = Type_ImageList;
                    header_Obj["img_width"] = data_main_.info.img_width_;
                    header_Obj["img_height"] = data_main_.info.img_height_;
                    header_Obj["img_format"] = data_main_.info.img_format_;
                    header_Obj["total_size"] = data_main_.info.img_width_ * data_main_.info.img_height_ * 3;
                    header_Obj["model_type"] = Model_LapNet;

                    this->write(toHeader(header_Obj) + data_main_.data_);
                    LogOut<<"Finished write---1"<<"\n";
                }
                else if(data_main_.info.message_type_ == Type_Video)
                {
                    header_Obj["msg_type"] = Type_Image;
                    header_Obj["img_width"] = data_main_.info.img_width_;
                    header_Obj["img_height"] = data_main_.info.img_height_;
                    header_Obj["img_format"] = data_main_.info.img_format_;
                    header_Obj["total_size"] = data_main_.info.img_width_ * data_main_.info.img_height_ * 3;
                    header_Obj["model_type"] = Model_LapNet;

                    this->write(toHeader(header_Obj) + data_main_.data_);
                    LogOut<<"Finished write---1"<<"\n";
                }
                else if(data_main_.info.message_type_ == Type_Train)
                {
                    qDebug() << "LapNet Train Msg !" << endl;
                }
                else if(data_main_.info.message_type_ == Type_File)
                {
                    QString string_file_type;

                    if(data_main_.info.file_type_ == File_OBJ)
                    {
                        string_file_type = "obj";
                    }
                    else if(data_main_.info.file_type_ == File_Dataset)
                    {
                        string_file_type = "dataset";
                    }

                    if(data_main_.info.file_name_.contains('/'))
                    {
                        QString folder_path = QDir::currentPath() + "/../Server_DataBase/received_" + string_file_type + "/lapnet_" + string_file_type + "/" + data_main_.info.file_name_.left(data_main_.info.file_name_.lastIndexOf('/'));

                        QDir dir(folder_path);

                        if(!dir.exists())
                        {
                            dir.mkpath(folder_path);
                        }
                    }

                    QFile file_recieved(QDir::currentPath() + "/../Server_DataBase/received_" + string_file_type + "/lapnet_" + string_file_type + "/" + data_main_.info.file_name_);

                    if(!file_recieved.open(QFile::WriteOnly))
                    {
                        LogOut<<"Couldn't write---1"<<"\n";
                    }

                    data_main_.toQFile(file_recieved);

                    file_recieved.close();

                    data_main_.clear();

                    header_Obj["msg_type"] = Type_FileFinished;
                    header_Obj["total_size"] = 0;
                    header_Obj["model_type"] = Model_LapNet;

                    this->write(toHeader(header_Obj) + data_main_.data_);

                    LogOut<<"Finished recieve---1"<<"\n";
                }
                else if(data_main_.info.message_type_ == Type_ReturnResult)
                {
                    //文件夹路径
                    QString modelDirPath = QFileInfo(QDir::currentPath() + "/../src/Python/LapNet/trained_model").absoluteFilePath();

                    if (modelDirPath.isEmpty())
                    {
                        return;
                    }
                    else
                    {
                        qDebug() << "modelDirPath=" << modelDirPath;
                    }

                    QString base_model_name = "LapNet_chkpt_better_epoch";

                    int max_model_epoch = 0;
                    int max_model_epoch_idx = 0;

                    QFileInfoList file_list = GetFileList(modelDirPath);

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

//                        data_main_.info.fromQFile(outfile);

//                        header_Obj["msg_type"] = Type_File;
//                        header_Obj["total_size"] = data_main_.info.total_size_;
//                        header_Obj["file_name"] = file_name;
//                        header_Obj["model_type"] = Model_LapNet;
//                        header_Obj["file_type"] = File_Free;

//                        this->write(toHeader(header_Obj) + data_main_.data_);

//                        this->waitForBytesWritten();
//                    }

                    QString filename = file_list[max_model_epoch_idx].absoluteFilePath();

                    QFile outfile(filename);

                    qDebug() << filename << endl;

                    filename = base_path + file_list[max_model_epoch_idx].absoluteFilePath().split(modelDirPath)[1];

                    qDebug() << filename << endl;

                    if(!outfile.open(QFile::ReadOnly))
                    {
                        std::cout << "打开文件失败！" << std::endl;

                        return;
                    }

                    data_main_.fromQFile(outfile);

                    header_Obj["msg_type"] = Type_File;
                    header_Obj["total_size"] = data_main_.info.total_size_;
                    header_Obj["file_name"] = filename;
                    header_Obj["model_type"] = Model_LapNet;
                    header_Obj["file_type"] = File_Free;

                    this->write(toHeader(header_Obj) + data_main_.data_);

                    this->waitForBytesWritten();
                }
            }
            else if(data_main_.info.model_type_ == Model_Yolov3)
            {
                header_Obj["msg_type"] = Type_BBox;
                header_Obj["bbox_num"] = data_main_.info.bbox_num_;
                header_Obj["total_size"] = data_main_.info.bbox_num_ * 5;
                header_Obj["model_type"] = Model_Yolov3;

                this->write(toHeader(header_Obj) + data_main_.data_);
                LogOut<<"Finished write---1"<<"\n";
            }
            else if(data_main_.info.model_type_ == Model_Darknet)
            {
                if(data_main_.info.message_type_ == Type_Image)
                {
                    header_Obj["msg_type"] = Type_BBox;
                    header_Obj["bbox_num"] = data_main_.info.bbox_num_;
                    header_Obj["total_size"] = data_main_.info.bbox_num_ * 5;
                    header_Obj["model_type"] = Model_Darknet;

                    this->write(toHeader(header_Obj) + data_main_.data_);
                    LogOut<<"Finished write---1"<<"\n";
                }
                else if(data_main_.info.message_type_ == Type_ImageList)
                {
                    header_Obj["msg_type"] = Type_BBoxList;
                    header_Obj["bbox_num"] = data_main_.info.bbox_num_;
                    header_Obj["total_size"] = data_main_.info.bbox_num_ * 5;
                    header_Obj["model_type"] = Model_Darknet;

                    this->write(toHeader(header_Obj) + data_main_.data_);
                    LogOut<<"Finished write---1"<<"\n";
                }
                else if(data_main_.info.message_type_ == Type_Frame)
                {
                    header_Obj["msg_type"] = Type_Image;
                    header_Obj["img_width"] = data_main_.info.img_width_;
                    header_Obj["img_height"] = data_main_.info.img_height_;
                    header_Obj["img_format"] = data_main_.info.img_format_;
                    header_Obj["total_size"] = data_main_.info.img_width_ * data_main_.info.img_height_ * 3;
                    header_Obj["model_type"] = Model_Darknet;

                    this->write(toHeader(header_Obj) + data_main_.data_);
                    LogOut<<"Finished write video frame---1"<<"\n";
                }
                else if(data_main_.info.message_type_ == Type_FrameBBox)
                {
                    header_Obj["msg_type"] = Type_FrameBBox;
                    header_Obj["bbox_num"] = data_main_.info.bbox_num_;
                    header_Obj["total_size"] = data_main_.info.bbox_num_ * 5;
                    header_Obj["model_type"] = Model_Darknet;

                    this->write(toHeader(header_Obj) + data_main_.data_);
                    LogOut<<"Finished write video frame---1"<<"\n";
                }
                else if(data_main_.info.message_type_ == Type_Video)
                {
                    header_Obj["msg_type"] = Type_Image;
                    header_Obj["img_width"] = data_main_.info.img_width_;
                    header_Obj["img_height"] = data_main_.info.img_height_;
                    header_Obj["img_format"] = data_main_.info.img_format_;
                    header_Obj["total_size"] = data_main_.info.img_width_ * data_main_.info.img_height_ * 3;
                    header_Obj["model_type"] = Model_Darknet;

                    this->write(toHeader(header_Obj) + data_main_.data_);
                    LogOut<<"Finished write video frame---1"<<"\n";
                }
                else if(data_main_.info.message_type_ == Type_VideoBBox)
                {
                    header_Obj["msg_type"] = Type_VideoBBox;
                    header_Obj["bbox_num"] = data_main_.info.bbox_num_;
                    header_Obj["total_size"] = data_main_.info.bbox_num_ * 5;
                    header_Obj["model_type"] = Model_Darknet;

                    this->write(toHeader(header_Obj) + data_main_.data_);
                    LogOut<<"Finished write video bbox---1"<<"\n";
                }
                else if(data_main_.info.message_type_ == Type_StopVideo)
                {
                    header_Obj["msg_type"] = Type_StopVideo;
                    header_Obj["total_size"] = 0;
                    header_Obj["model_type"] = Model_Darknet;

                    this->write(toHeader(header_Obj) + data_main_.data_);
                    LogOut<<"Finished stop video---1"<<"\n";
                }
                else if(data_main_.info.message_type_ == Type_Train)
                {
                    qDebug() << "DarkNet Train Msg !" << endl;
                    int a__ = 0;
                }
                else if(data_main_.info.message_type_ == Type_File)
                {
                    QString string_file_type;

                    if(data_main_.info.file_type_ == File_OBJ)
                    {
                        string_file_type = "obj";
                    }
                    else if(data_main_.info.file_type_ == File_Dataset)
                    {
                        string_file_type = "dataset";
                    }

                    if(data_main_.info.file_name_.contains('/'))
                    {
                        QString folder_path = QDir::currentPath() + "/../Server_DataBase/received_" + string_file_type + "/darknet_" + string_file_type + "/" + data_main_.info.file_name_.left(data_main_.info.file_name_.lastIndexOf('/'));

                        QDir dir(folder_path);

                        if(!dir.exists())
                        {
                            dir.mkpath(folder_path);
                        }
                    }

                    QFile file_recieved(QDir::currentPath() + "/../Server_DataBase/received_" + string_file_type + "/darknet_" + string_file_type + "/" + data_main_.info.file_name_);

                    if(!file_recieved.open(QFile::WriteOnly))
                    {
                        LogOut<<"Couldn't write---1"<<"\n";
                    }

                    data_main_.toQFile(file_recieved);

                    file_recieved.close();

                    data_main_.clear();

                    header_Obj["msg_type"] = Type_FileFinished;
                    header_Obj["total_size"] = 0;
                    header_Obj["model_type"] = Model_Darknet;

                    this->write(toHeader(header_Obj) + data_main_.data_);
                    LogOut<<"Finished recieve---1"<<"\n";
                }
                else if(data_main_.info.message_type_ == Type_ReturnResult)
                {
                    int a__ = 0;
                }
            }
            else if(data_main_.info.model_type_ == Model_Free)
            {
                if(data_main_.info.message_type_ == Type_ReceivedFileMsg)
                {
                    QFile file_sent(QDir::currentPath() + "/../Server_DataBase/message_set/received_files_list.txt");

                    if(!file_sent.open(QFile::ReadOnly))
                    {
                        LogOut<<"Couldn't read---1"<<"\n";
                    }

                    data_main_.fromQFile(file_sent);

                    file_sent.close();

                    header_Obj["msg_type"] = Type_File;
                    header_Obj["total_size"] = data_main_.info.total_size_;
                    header_Obj["file_name"] = "../Client_DataBase/message_set/received_files_list.txt";
                    header_Obj["model_type"] = Model_Free;
                    header_Obj["file_type"] = File_ReceivedFileMsg;

                    this->write(toHeader(header_Obj) + data_main_.data_);

                    LogOut<<"Finished sent---1"<<"\n";
                }
                else if(data_main_.info.message_type_ == Type_ClearAll)
                {
                    delDir("../Server_DataBase");

                    QDir dir_;

                    dir_.mkpath("../Server_DataBase/message_set/");
                }
            }
        }

        state_ = State_WaitConnection;

        emit processFinished();
    }

    LogOut<<data_main_.data_.size()<<" "<<header_array_.size()<<"\n";

    LogClean
}

QByteArray Connection::messageFlag(std::string title_msg)
{
    return QString::fromStdString("=" + title_msg + "-").toUtf8();
}

QByteArray Connection::toHeader(QJsonObject header_Obj)
{
    QJsonDocument doc;
    doc.setObject(header_Obj);

    QByteArray array;
    array.push_back(doc.toJson());
//    array.push_back(doc.toJson().toBase64());
    array.append(messageFlag("HeaderEnd"));

    return array;
}

//QByteArray Connection::toImageHeader(int width, int height, int format, ModelType model_type)
//{
//    QByteArray array;
//    QJsonDocument doc;
//    QJsonObject obj;
//    obj["msg_type"] = Type_Image;
//    obj["img_width"] = width;
//    obj["img_height"] = height;
//    obj["img_format"] = format;
//    obj["total_size"] = width * height * 3;
//    obj["model_type"] = model_type;
//    doc.setObject(obj);
//    array.push_back(doc.toJson());
//    array.append(messageFlag("HeaderEnd"));

//    return array;
//}

//QByteArray Connection::toImageListHeader(int width, int height, int format, ModelType model_type)
//{
//    QByteArray array;
//    QJsonDocument doc;
//    QJsonObject obj;
//    obj["msg_type"] = Type_ImageList;
//    obj["img_width"] = width;
//    obj["img_height"] = height;
//    obj["img_format"] = format;
//    obj["total_size"] = width * height * 3;
//    obj["model_type"] = model_type;
//    doc.setObject(obj);
//    array.push_back(doc.toJson());
//    array.append(messageFlag("HeaderEnd"));

//    return array;
//}

//QByteArray Connection::toFrameHeader(QString conda_env_name, ModelType model_type)
//{
//    QByteArray array;
//    QJsonDocument doc;
//    QJsonObject obj;
//    obj["msg_type"] = Type_Frame;
//    obj["conda_env_name"] = conda_env_name;
//    obj["total_size"] = 0;
//    obj["model_type"] = model_type;
//    doc.setObject(obj);
//    array.push_back(doc.toJson());
//    array.append(messageFlag("HeaderEnd"));

//    return array;
//}

//QByteArray Connection::toVideoHeader(QString conda_env_name, ModelType model_type)
//{
//    QByteArray array;
//    QJsonDocument doc;
//    QJsonObject obj;
//    obj["msg_type"] = Type_Video;
//    obj["total_size"] = 0;
//    obj["conda_env_name"] = conda_env_name;
//    obj["model_type"] = model_type;
//    doc.setObject(obj);
//    array.push_back(doc.toJson());
//    array.append(messageFlag("HeaderEnd"));

//    return array;
//}

//QByteArray Connection::toBBoxHeader(int bbox_num, ModelType model_type)
//{
//    QByteArray array;
//    QJsonDocument doc;
//    QJsonObject obj;
//    obj["msg_type"] = Type_BBox;
//    obj["bbox_num"] = bbox_num;
//    obj["total_size"] = bbox_num * 5;
//    obj["model_type"] = model_type;
//    doc.setObject(obj);
//    array.push_back(doc.toJson());
//    array.append(messageFlag("HeaderEnd"));

//    return array;
//}

//QByteArray Connection::toBBoxListHeader(int bbox_num, ModelType model_type)
//{
//    QByteArray array;
//    QJsonDocument doc;
//    QJsonObject obj;
//    obj["msg_type"] = Type_BBoxList;
//    obj["bbox_num"] = bbox_num;
//    obj["total_size"] = bbox_num * 5;
//    obj["model_type"] = model_type;
//    doc.setObject(obj);
//    array.push_back(doc.toJson());
//    array.append(messageFlag("HeaderEnd"));

//    return array;
//}

//QByteArray Connection::toFrameBBoxHeader(int bbox_num, ModelType model_type)
//{
//    QByteArray array;
//    QJsonDocument doc;
//    QJsonObject obj;
//    obj["msg_type"] = Type_FrameBBox;
//    obj["bbox_num"] = bbox_num;
//    obj["total_size"] = bbox_num * 5;
//    obj["model_type"] = model_type;
//    doc.setObject(obj);
//    array.push_back(doc.toJson());
//    array.append(messageFlag("HeaderEnd"));

//    return array;
//}

//QByteArray Connection::toVideoBBoxHeader(int bbox_num, ModelType model_type)
//{
//    QByteArray array;
//    QJsonDocument doc;
//    QJsonObject obj;
//    obj["msg_type"] = Type_VideoBBox;
//    obj["bbox_num"] = bbox_num;
//    obj["total_size"] = bbox_num * 5;
//    obj["model_type"] = model_type;
//    doc.setObject(obj);
//    array.push_back(doc.toJson());
//    array.append(messageFlag("HeaderEnd"));

//    return array;
//}

//QByteArray Connection::toStopVideoHeader(ModelType model_type)
//{
//    QByteArray array;
//    QJsonDocument doc;
//    QJsonObject obj;
//    obj["msg_type"] = Type_StopVideo;
//    obj["total_size"] = 0;
//    obj["model_type"] = model_type;
//    doc.setObject(obj);
//    array.push_back(doc.toJson());
//    array.append(messageFlag("HeaderEnd"));

//    return array;
//}

//QByteArray Connection::toFileHeader(int total_size, QString file_name, ModelType model_type, FileType file_type)
//{
//    QByteArray array;
//    QJsonDocument doc;
//    QJsonObject obj;
//    obj["msg_type"] = Type_File;
//    obj["total_size"] = total_size;
//    obj["file_name"] = file_name;
//    obj["model_type"] = model_type;
//    obj["file_type"] = file_type;
//    doc.setObject(obj);
//    array.push_back(doc.toJson());
//    array.append(messageFlag("HeaderEnd"));

//    return array;
//}

//QByteArray Connection::toFileFinishedHeader(ModelType model_type)
//{
//    QByteArray array;
//    QJsonDocument doc;
//    QJsonObject obj;
//    obj["msg_type"] = Type_FileFinished;
//    obj["total_size"] = 0;
//    obj["model_type"] = model_type;
//    doc.setObject(obj);
//    array.push_back(doc.toJson());
//    array.append(messageFlag("HeaderEnd"));

//    return array;
//}

//QByteArray Connection::toCreateDataHeader(int create_data_num, int max_obj_num_per_img, ModelType model_type)
//{
//    QByteArray array;
//    QJsonDocument doc;
//    QJsonObject obj;
//    obj["msg_type"] = Type_CreateData;
//    obj["total_size"] = 0;
//    obj["model_type"] = model_type;
//    obj["create_data_num"] = create_data_num;
//    obj["max_obj_num_per_img"] = max_obj_num_per_img;
//    doc.setObject(obj);
//    array.push_back(doc.toJson());
//    array.append(messageFlag("HeaderEnd"));

//    return array;
//}

//QByteArray Connection::toEnhancementHeader(QString conda_env_name, int enhancement_factor, ModelType model_type)
//{
//    QByteArray array;
//    QJsonDocument doc;
//    QJsonObject obj;
//    obj["msg_type"] = Type_Enhancement;
//    obj["total_size"] = 0;
//    obj["conda_env_name"] = conda_env_name;
//    obj["model_type"] = model_type;
//    obj["enhancement_factor"] = enhancement_factor;
//    doc.setObject(obj);
//    array.push_back(doc.toJson());
//    array.append(messageFlag("HeaderEnd"));

//    return array;
//}

//QByteArray Connection::toDataChangeHeader(QString conda_env_name, int train_percent, int label_num, ModelType model_type)
//{
//    QByteArray array;
//    QJsonDocument doc;
//    QJsonObject obj;
//    obj["msg_type"] = Type_DataChange;
//    obj["total_size"] = 0;
//    obj["conda_env_name"] = conda_env_name;
//    obj["train_percent"] = train_percent;
//    obj["label_num"] = label_num;
//    obj["model_type"] = model_type;
//    doc.setObject(obj);
//    array.push_back(doc.toJson());
//    array.append(messageFlag("HeaderEnd"));

//    return array;
//}

//QByteArray Connection::toTrainHeader(QString conda_env_name, ModelType model_type)
//{
//    QByteArray array;
//    QJsonDocument doc;
//    QJsonObject obj;
//    obj["msg_type"] = Type_Train;
//    obj["total_size"] = 0;
//    obj["conda_env_name"] = conda_env_name;
//    obj["model_type"] = model_type;
//    doc.setObject(obj);
//    array.push_back(doc.toJson());
//    array.append(messageFlag("HeaderEnd"));

//    return array;
//}

//QByteArray Connection::toStopTrainHeader(ModelType model_type)
//{
//    QByteArray array;
//    QJsonDocument doc;
//    QJsonObject obj;
//    obj["msg_type"] = Type_StopTrain;
//    obj["total_size"] = 0;
//    obj["model_type"] = model_type;
//    doc.setObject(obj);
//    array.push_back(doc.toJson());
//    array.append(messageFlag("HeaderEnd"));

//    return array;
//}

//QByteArray Connection::toValidHeader(QString conda_env_name, ModelType model_type)
//{
//    QByteArray array;
//    QJsonDocument doc;
//    QJsonObject obj;
//    obj["msg_type"] = Type_Valid;
//    obj["total_size"] = 0;
//    obj["conda_env_name"] = conda_env_name;
//    obj["model_type"] = model_type;
//    doc.setObject(obj);
//    array.push_back(doc.toJson());
//    array.append(messageFlag("HeaderEnd"));

//    return array;
//}

//QByteArray Connection::toReturnResultHeader(ModelType model_type)
//{
//    QByteArray array;
//    QJsonDocument doc;
//    QJsonObject obj;
//    obj["msg_type"] = Type_ReturnResult;
//    obj["total_size"] = 0;
//    obj["model_type"] = model_type;
//    doc.setObject(obj);
//    array.push_back(doc.toJson());
//    array.append(messageFlag("HeaderEnd"));

//    return array;
//}

//QByteArray Connection::toReceivedFileMsgHeader()
//{
//    QByteArray array;
//    QJsonDocument doc;
//    QJsonObject obj;
//    obj["msg_type"] = Type_ReceivedFileMsg;
//    obj["total_size"] = 0;
//    obj["model_type"] = Model_Free;
//    doc.setObject(obj);
//    array.push_back(doc.toJson());
//    array.append(messageFlag("HeaderEnd"));

//    return array;
//}

//QByteArray Connection::toClearAllHeader()
//{
//    QByteArray array;
//    QJsonDocument doc;
//    QJsonObject obj;
//    obj["msg_type"] = Type_ClearAll;
//    obj["total_size"] = 0;
//    obj["model_type"] = Model_Free;
//    doc.setObject(obj);
//    array.push_back(doc.toJson());
//    array.append(messageFlag("HeaderEnd"));

//    return array;
//}

//QByteArray Connection::toInitModelHeader(QString conda_env_name, bool load_net, double thresh)
//{
//    QByteArray array;
//    QJsonDocument doc;
//    QJsonObject obj;
//    obj["msg_type"] = Type_InitModel;
//    obj["total_size"] = 0;
//    obj["conda_env_name"] = conda_env_name;
//    obj["load_net"] = load_net;
//    obj["thresh"] = thresh;
//    obj["model_type"] = Model_Free;
//    doc.setObject(obj);
//    array.push_back(doc.toJson());
//    array.append(messageFlag("HeaderEnd"));

//    return array;
//}

QFileInfoList Connection::GetFileList(QString path)
{
    QDir dir(path);
    QFileInfoList file_list = dir.entryInfoList(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    QFileInfoList folder_list = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);

    for(int i = 0; i != folder_list.size(); i++)
    {
         QString name = folder_list.at(i).absoluteFilePath();
         QFileInfoList child_file_list = GetFileList(name);
         file_list.append(child_file_list);
    }

    return file_list;
}


bool Connection::cpDir(QString srcPath, QString dstPath)
{
    QDir parentDstDir(QFileInfo(dstPath).path());

    if(!parentDstDir.mkpath(QFileInfo(dstPath).fileName()))
    {
        return false;
    }

    QDir srcDir(srcPath);
    foreach(QFileInfo info, srcDir.entryInfoList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot))
    {
        QString srcItemPath = srcPath + "/" + info.fileName();
        QString dstItemPath = dstPath + "/" + info.fileName();
        if(info.isDir())
        {
            if(!cpDir(srcItemPath, dstItemPath))
            {
                return false;
            }
        }
        else if(info.isFile())
        {
            if(!QFile::copy(srcItemPath, dstItemPath))
            {
                return false;
            }
        }
        else
        {
            qDebug() << "Unhandled item" << info.filePath() << "in cpDir";
        }
    }

    return true;
}

bool Connection::delDir(QString dirPath){
    QDir dir(dirPath);

    dir.removeRecursively();

    return true;
}

bool Connection::is_file_updated(QString file_path)
{
    // Todo
    QFile file(file_path);
    if(!file.exists())
    {
        return false;
    }

    return false;
}

void Connection::ConnectedData::clear()
{
    info.img_width_ = 0;
    info.img_height_ = 0;
    info.img_format_ = 0;
    info.bbox_num_ = 0;

    info.total_size_ = 0;

    info.message_type_ = Type_Free;

    info.model_type_ = Model_Free;

    info.file_type_ = File_Free;

    info.file_name_.clear();

    info.create_data_num_ = 0;

    info.max_obj_num_per_img_ = 0;

    info.enhancement_factor_ = 0;

    info.train_percent_ = 0;

    info.label_num_ = 0;

    info.conda_env_name_.clear();

    info.image_dir_.clear();
    info.output_dir_.clear();

    data_.clear();
}

void Connection::ConnectedData::fromQImage(const QImage &img)
{
    this->info.img_width_ = img.width();
    this->info.img_height_ = img.height();
    this->info.img_format_ = 3;

//    this->data_.reserve(this->img_width_*this->img_height_*3);
    this->data_.resize(this->info.img_width_*this->info.img_height_*3);
    for(int i=0; i < img.width(); i++)
    {
        for(int j=0; j < img.height(); j++)
        {
            QRgb rgb = img.pixel(i,j);
            this->data_[3 * (j * img.width() + i) + 0] = qRed(rgb);
            this->data_[3 * (j * img.width() + i) + 1] = qGreen(rgb);
            this->data_[3 * (j * img.width() + i) + 2] = qBlue(rgb);

        }
    }
}

void Connection::ConnectedData::toQImage(QImage &img) const
{
    img = QImage(this->info.img_width_,this->info.img_height_, QImage::Format_RGB888);

    for(int i=0; i < img.width(); i++)
    {
        for(int j=0; j < img.height(); j++)
        {
            int r = this->data_[3 * (j * img.width() + i) + 0];
            int g = this->data_[3 * (j * img.width() + i) + 1];
            int b = this->data_[3 * (j * img.width() + i) + 2];

            img.setPixel(i,j,qRgb(r,g,b));
        }
    }
}

std::vector<std::vector<int>> Connection::ConnectedData::toBBox()
{
    std::vector<std::vector<int>> bbox_result;

    bbox_result.resize(info.bbox_num_);

    for(int i = 0; i < info.bbox_num_; ++i)
    {
        bbox_result[i].emplace_back(getBBoxX(i));
        bbox_result[i].emplace_back(getBBoxY(i));
        bbox_result[i].emplace_back(getBBoxW(i));
        bbox_result[i].emplace_back(getBBoxH(i));
        bbox_result[i].emplace_back(getBBoxLabel(i));
    }

    return bbox_result;
}

void Connection::ConnectedData::fromQFile(QFile &file)
{
    this->data_.resize(0);

    this->data_ = file.readAll().toBase64();

    this->info.total_size_ = this->data_.size();
}

void Connection::ConnectedData::toQFile(QFile &file)
{
    file.resize(0);
    file.write(QByteArray::fromBase64(this->data_));
}

}
