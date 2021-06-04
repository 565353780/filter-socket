#ifndef CONNECTION_H
#define CONNECTION_H
#include <QTcpSocket>
#include <QByteArray>
#include <memory>
#include <iostream>
#include <QFile>
#include <QDir>
#include <QString>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

class QImage;
namespace FilterSocket {

class FilterInterface;
class Connection : public QTcpSocket
{
    Q_OBJECT
public:
    enum ConnectionState
    {
        State_WaitConnection = 0,
        State_ReadHeader = 1,
        State_ReadData = 2,
        State_Finished = 3
    };

    enum MessageType
    {
        Type_Free = 0,
        Type_Image = 1,
        Type_ImageList = 2,
        Type_BBox = 3,
        Type_BBoxList = 4,
        Type_File = 5,
        Type_FileFinished = 6,
        Type_CreateData = 7,
        Type_Enhancement = 8,
        Type_DataChange = 9,
        Type_Train = 10,
        Type_Valid = 11,
        Type_StopTrain = 12,
        Type_ReturnResult = 13,
        Type_ReceivedFileMsg = 14,
        Type_ClearAll = 15,
        Type_InitModel = 16,
        Type_Frame = 17,
        Type_FrameBBox = 18,
        Type_Video = 19,
        Type_VideoBBox = 20,
        Type_StopVideo = 21,
        Type_Test = 22
    };

    enum ModelType
    {
        Model_Free = 0,
        Model_LapNet = 1,
        Model_Yolov3 = 2,
        Model_Darknet = 3,
        Model_PolyYolo = 4
    };

    enum FileType
    {
        File_Free = 0,
        File_OBJ = 1,
        File_Dataset = 2,
        File_ReceivedFileMsg = 3
    };

    struct Info
    {
        int img_width_{0};
        int img_height_{0};
        int img_format_{0};
        int bbox_num_{0};

        int total_size_{0};

        int message_type_{Type_Free};

        int model_type_{Model_Free};

        int file_type_{File_Free};

        QString file_name_;

        int create_data_num_{0};

        int max_obj_num_per_img_{0};

        int enhancement_factor_{0};

        int train_percent_{0};

        int label_num_{0};

        QString conda_env_name_;

        QString image_dir_;
        QString output_dir_;
    };

    struct ConnectedData
    {
        Info info;

        QByteArray data_;

        inline int getRed(int i, int j)
        {
            return data_[3 *(j * info.img_width_ + i)];
        }
        inline int getGreen(int i, int j)
        {
            return data_[3 * (j * info.img_width_ + i) + 1];
        }

        inline int getBlue(int i, int j)
        {
            return data_[3 * (j * info.img_width_ + i) + 2];
        }

        inline int getBBoxX(int i)
        {
            return getInt(5 * i);
        }

        inline int getBBoxY(int i)
        {
            return getInt(5 * i + 1);
        }

        inline int getBBoxW(int i)
        {
            return getInt(5 * i + 2);
        }

        inline int getBBoxH(int i)
        {
            return getInt(5 * i + 3);
        }

        inline int getBBoxLabel(int i)
        {
            return getInt(5 * i + 4);
        }

        inline void setPixel(int i,int j, int r, int g, int b)
        {
            data_[3 *(j * info.img_width_ + i)] = r;
            data_[3 *(j * info.img_width_ + i) + 1] = g;
            data_[3 *(j * info.img_width_ + i) + 2] = b;
        }

        inline void resetBBoxNum(int reset_bbox_num)
        {
            info.bbox_num_ = reset_bbox_num;
            data_.resize(info.bbox_num_ * 4 * 5);
        }

        inline void setBBox(int i, int x, int y, int w, int h, int label)
        {
            setInt(5 * i, x);
            setInt(5 * i + 1, y);
            setInt(5 * i + 2, w);
            setInt(5 * i + 3, h);
            setInt(5 * i + 4, label);
        }

        inline void setInt(int i, int data)
        {
            data_[4 * i] = (uchar)(0x000000ff & data);
            data_[4 * i + 1] = (uchar)((0x0000ff00 & data) >> 8);
            data_[4 * i + 2] = (uchar)((0x00ff0000 & data) >> 16);
            data_[4 * i + 3] = (uchar)((0xff000000 & data) >> 24);
        }

        inline int getInt(int i)
        {
            int data = data_[4 * i] & 0x000000ff;
            data |= ((data_[4 * i + 1] << 8) & 0x0000ff00);
            data |= ((data_[4 * i + 2] << 16) & 0x00ff0000);
            data |= ((data_[4 * i + 3] << 24) & 0xff000000);

            return data;
        }

        void clear();
        void fromQImage(const QImage &img);
        void toQImage(QImage &img) const;

        std::vector<std::vector<int>> toBBox();

        void fromQFile(QFile &file);
        void toQFile(QFile &file);

        void outputInfo();
    };
    Connection(QObject *parent =nullptr);

    void setClient(bool t) {is_client_ = t;}
    void setFilter(std::shared_ptr<FilterInterface> filter);
signals:
    void processFinished();

    void updateFileProgress();
public slots:
    void processData();
public:
    static QByteArray messageFlag(std::string title_msg);

    static QByteArray toHeader(QJsonObject header_Obj);

    static QByteArray toImageHeader(int width,int height, int format, ModelType model_type=Model_Free);

    static QByteArray toImageListHeader(int width,int height, int format, ModelType model_type=Model_Free);

    static QByteArray toFrameHeader(QString conda_env_name, ModelType model_type=Model_Free);

    static QByteArray toVideoHeader(QString conda_env_name, ModelType model_type=Model_Free);

    static QByteArray toBBoxHeader(int bbox_num, ModelType model_type=Model_Free);

    static QByteArray toBBoxListHeader(int bbox_num, ModelType model_type=Model_Free);

    static QByteArray toFrameBBoxHeader(int bbox_num, ModelType model_type=Model_Free);

    static QByteArray toVideoBBoxHeader(int bbox_num, ModelType model_type=Model_Free);

    static QByteArray toStopVideoHeader(ModelType model_type=Model_Free);

    static QByteArray toFileHeader(int total_size, QString file_name, ModelType model_type=Model_Free, FileType file_type=File_Free);

    static QByteArray toFileFinishedHeader(ModelType model_type=Model_Free);

    static QByteArray toCreateDataHeader(int create_data_num, int max_obj_num_per_img, ModelType model_type=Model_Free);

    static QByteArray toEnhancementHeader(QString conda_env_name, int enhancement_factor, ModelType model_type=Model_Free);

    static QByteArray toDataChangeHeader(QString conda_env_name, int train_percent, int label_num, ModelType model_type=Model_Free);

    static QByteArray toTrainHeader(QString conda_env_name, ModelType model_type=Model_Free);

    static QByteArray toStopTrainHeader(ModelType model_type=Model_Free);

    static QByteArray toValidHeader(QString conda_env_name, ModelType model_type=Model_Free);

    static QByteArray toReturnResultHeader(ModelType model_type=Model_Free);

    static QByteArray toReceivedFileMsgHeader();

    static QByteArray toClearAllHeader();

    static QByteArray toInitModelHeader(QString conda_env_name, bool load_net, double thresh);

    static QFileInfoList GetFileList(QString path);

    static bool cpDir(QString srcPath, QString dstPath);

    static bool delDir(QString dirName);

    static bool is_file_updated(QString file_path);

public:
    ConnectedData &getData() {return data_main_;}
private:
    QByteArray header_array_;
    int state_{State_WaitConnection};
    ConnectedData data_main_;
    ConnectedData data_res_;
    int is_client_{false};
    std::shared_ptr<FilterInterface> filter_;
};

class FilterInterface
{
public:
    FilterInterface(){}
    virtual ~FilterInterface(){}
    virtual int process(Connection::ConnectedData &data){return 0;}
};
}
#endif // CONNECTION_H
