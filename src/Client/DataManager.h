#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <QObject>
#include <QStandardItemModel>

#include "DatabaseManager/DatabaseManager.h"

class QDir;

class DataManager : public QObject
{
    Q_OBJECT

public:
    enum NetworkType
    {
        Yolov3Region = 0,
        Yolov3Component = 1,
        ResNetClassification = 2
    };

    explicit DataManager(QObject *parent = nullptr);
    ~DataManager();

    void init(QString db_path="");

    void initSourceImageModel();
    void initMaskModel();
    void initPredictModel();
    void initAllModel();

    void updateSourceImageModel();

    void clearSourceImage();

    void insertToSourceImageModel(QString ImageID);

    void updateMaskModel(QString ImageID);
    void updatePredictModel(QString ImageID);
    void updateMaskAndPredictModel(QString ImageID);
    void updateMaskAndPredictModel(int row_index);

    void outputSourceImage(QString ImageID);

    void outputMaskBoundingBox(QString ImageID);

    void outputPredictBoundingBox(QString ImageID);

    void outputAll(QString ImageID);

    void loadConfig(QString filename);

    QJsonObject sourceImageJson(QString ImageID,
                                QString ImagePath,
                                QString ImageWidth,
                                QString ImageHeight,
                                QString ImportTime,
                                QString isYolov3RegionMasked,
                                QString Yolov3RegionMaskID,
                                QString isYolov3ComponentMasked,
                                QString Yolov3ComponentMaskID,
                                QString isResNetClassificationMasked,
                                QString ResNetClassificationMaskID,
                                QString isYolov3RegionPredicted,
                                QString isYolov3ComponentPredicted,
                                QString isResNetClassificationPredicted);

    QJsonObject sourceImageWithOperatorJson(QString ImageIDOperator,
                                            QString ImageID,
                                            QString ImagePathOperator,
                                            QString ImagePath,
                                            QString ImageWidthOperator,
                                            QString ImageWidth,
                                            QString ImageHeightOperator,
                                            QString ImageHeight,
                                            QString ImportTimeOperator,
                                            QString ImportTime,
                                            QString isYolov3RegionMaskedOperator,
                                            QString isYolov3RegionMasked,
                                            QString Yolov3RegionMaskIDOperator,
                                            QString Yolov3RegionMaskID,
                                            QString isYolov3ComponentMaskedOperator,
                                            QString isYolov3ComponentMasked,
                                            QString Yolov3ComponentMaskIDOperator,
                                            QString Yolov3ComponentMaskID,
                                            QString isResNetClassificationMaskedOperator,
                                            QString isResNetClassificationMasked,
                                            QString ResNetClassificationMaskIDOperator,
                                            QString ResNetClassificationMaskID,
                                            QString isYolov3RegionPredictedOperator,
                                            QString isYolov3RegionPredicted,
                                            QString isYolov3ComponentPredictedOperator,
                                            QString isYolov3ComponentPredicted,
                                            QString isResNetClassificationPredictedOperator,
                                            QString isResNetClassificationPredicted);

    QJsonObject maskBoundingBoxJson(QString ImageID,
                                    QString ID,
                                    QString Time,
                                    QString label,
                                    QString x1,
                                    QString x2,
                                    QString y1,
                                    QString y2);

    QJsonObject maskBoundingBoxWithOperatorJson(QString ImageIDOperator,
                                                QString ImageID,
                                                QString IDOperator,
                                                QString ID,
                                                QString TimeOperator,
                                                QString Time,
                                                QString labelOperator,
                                                QString label,
                                                QString x1Operator,
                                                QString x1,
                                                QString x2Operator,
                                                QString x2,
                                                QString y1Operator,
                                                QString y1,
                                                QString y2Operator,
                                                QString y2);

    QJsonObject predictBoundingBoxJson(QString ImageID,
                                       QString ID,
                                       QString Time,
                                       QString label,
                                       QString x1,
                                       QString x2,
                                       QString y1,
                                       QString y2,
                                       QString score);

    QJsonObject predictBoundingBoxWithOperatorJson(QString ImageIDOperator,
                                                   QString ImageID,
                                                   QString IDOperator,
                                                   QString ID,
                                                   QString TimeOperator,
                                                   QString Time,
                                                   QString labelOperator,
                                                   QString label,
                                                   QString x1Operator,
                                                   QString x1,
                                                   QString x2Operator,
                                                   QString x2,
                                                   QString y1Operator,
                                                   QString y1,
                                                   QString y2Operator,
                                                   QString y2,
                                                   QString scoreOperator,
                                                   QString score);

    QMap<QString, QString> jsonToMap(QJsonObject json_Obj);

    QList<QMap<QString, QString>> jsonToMapList(QJsonObject json_Obj);

    bool insertSourceImage(QJsonObject insert_Obj);

    bool insertSourceImage(QString ImagePath);

    bool reviseSourceImage(QString ImageID,
                           QString ImagePath,
                           QString ImageWidth,
                           QString ImageHeight,
                           QString ImportTime,
                           QString isYolov3RegionMasked,
                           QString Yolov3RegionMaskID,
                           QString isYolov3ComponentMasked,
                           QString Yolov3ComponentMaskID,
                           QString isResNetClassificationMasked,
                           QString ResNetClassificationMaskID,
                           QString isYolov3RegionPredicted,
                           QString isYolov3ComponentPredicted,
                           QString isResNetClassificationPredicted);

    bool reviseSourceImage(QJsonObject revise_Obj);

    bool querySourceImage(QString ImageIDOperator,
                          QString ImageID,
                          QString ImagePathOperator,
                          QString ImagePath,
                          QString ImageWidthOperator,
                          QString ImageWidth,
                          QString ImageHeightOperator,
                          QString ImageHeight,
                          QString ImportTimeOperator,
                          QString ImportTime,
                          QString isYolov3RegionMaskedOperator,
                          QString isYolov3RegionMasked,
                          QString Yolov3RegionMaskIDOperator,
                          QString Yolov3RegionMaskID,
                          QString isYolov3ComponentMaskedOperator,
                          QString isYolov3ComponentMasked,
                          QString Yolov3ComponentMaskIDOperator,
                          QString Yolov3ComponentMaskID,
                          QString isResNetClassificationMaskedOperator,
                          QString isResNetClassificationMasked,
                          QString ResNetClassificationMaskIDOperator,
                          QString ResNetClassificationMaskID,
                          QString isYolov3RegionPredictedOperator,
                          QString isYolov3RegionPredicted,
                          QString isYolov3ComponentPredictedOperator,
                          QString isYolov3ComponentPredicted,
                          QString isResNetClassificationPredictedOperator,
                          QString isResNetClassificationPredicted);

    bool querySourceImage(QJsonObject query_Obj);

    bool deleteSourceImage(QString ImageIDOperator,
                           QString ImageID,
                           QString ImagePathOperator,
                           QString ImagePath,
                           QString ImageWidthOperator,
                           QString ImageWidth,
                           QString ImageHeightOperator,
                           QString ImageHeight,
                           QString ImportTimeOperator,
                           QString ImportTime,
                           QString isYolov3RegionMaskedOperator,
                           QString isYolov3RegionMasked,
                           QString Yolov3RegionMaskIDOperator,
                           QString Yolov3RegionMaskID,
                           QString isYolov3ComponentMaskedOperator,
                           QString isYolov3ComponentMasked,
                           QString Yolov3ComponentMaskIDOperator,
                           QString Yolov3ComponentMaskID,
                           QString isResNetClassificationMaskedOperator,
                           QString isResNetClassificationMasked,
                           QString ResNetClassificationMaskIDOperator,
                           QString ResNetClassificationMaskID,
                           QString isYolov3RegionPredictedOperator,
                           QString isYolov3RegionPredicted,
                           QString isYolov3ComponentPredictedOperator,
                           QString isYolov3ComponentPredicted,
                           QString isResNetClassificationPredictedOperator,
                           QString isResNetClassificationPredicted);

    bool deleteSourceImage(QJsonObject delete_Obj);

    bool insertMaskResult(NetworkType network_type,
                          QString ImageID,
                          QString ID,
                          QString Time,
                          QString label,
                          QString x1,
                          QString x2,
                          QString y1,
                          QString y2);

    bool insertMaskResult(NetworkType network_type, QJsonObject insert_Obj);

    bool queryMaskResult(NetworkType network_type,
                         QString ImageIDOperator,
                         QString ImageID,
                         QString IDOperator,
                         QString ID,
                         QString TimeOperator,
                         QString Time,
                         QString labelOperator,
                         QString label,
                         QString x1Operator,
                         QString x1,
                         QString x2Operator,
                         QString x2,
                         QString y1Operator,
                         QString y1,
                         QString y2Operator,
                         QString y2);

    bool queryMaskResult(NetworkType network_type, QJsonObject query_Obj);

    bool deleteMaskResult(NetworkType network_type,
                          QString ImageIDOperator,
                          QString ImageID,
                          QString IDOperator,
                          QString ID,
                          QString TimeOperator,
                          QString Time,
                          QString labelOperator,
                          QString label,
                          QString x1Operator,
                          QString x1,
                          QString x2Operator,
                          QString x2,
                          QString y1Operator,
                          QString y1,
                          QString y2Operator,
                          QString y2);

    bool deleteMaskResult(NetworkType network_type, QJsonObject delete_Obj);

    bool insertPredictResult(NetworkType network_type,
                             QString ImageID,
                             QString ID,
                             QString Time,
                             QString label,
                             QString x1,
                             QString x2,
                             QString y1,
                             QString y2,
                             QString score);

    bool insertPredictResult(NetworkType network_type, QJsonObject insert_Obj);

    bool queryPredictResult(NetworkType network_type,
                            QString ImageIDOperator,
                            QString ImageID,
                            QString IDOperator,
                            QString ID,
                            QString TimeOperator,
                            QString Time,
                            QString labelOperator,
                            QString label,
                            QString x1Operator,
                            QString x1,
                            QString x2Operator,
                            QString x2,
                            QString y1Operator,
                            QString y1,
                            QString y2Operator,
                            QString y2,
                            QString scoreOperator,
                            QString score);

    bool queryPredictResult(NetworkType network_type, QJsonObject query_Obj);

    bool deletePredictResult(NetworkType network_type,
                             QString ImageIDOperator,
                             QString ImageID,
                             QString IDOperator,
                             QString ID,
                             QString TimeOperator,
                             QString Time,
                             QString labelOperator,
                             QString label,
                             QString x1Operator,
                             QString x1,
                             QString x2Operator,
                             QString x2,
                             QString y1Operator,
                             QString y1,
                             QString y2Operator,
                             QString y2,
                             QString scoreOperator,
                             QString score);

    bool deletePredictResult(NetworkType network_type, QJsonObject delete_Obj);

    QList<QJsonObject> getQueryResultList();

    QString getFileHash(QString file_path);

    bool insertFromMaskJsonObj(NetworkType network_type, QString ImageID, QJsonObject mask_json_Obj, double min_x, double min_y, double max_x, double max_y);

    bool insertFromMaskJsonFile(NetworkType network_type, QString ImageID, QString mask_json_file_path, double min_x, double min_y, double max_x, double max_y);

    bool insertFromImageFile(NetworkType network_type, QString ImageID, QString ImagePath);

    bool insertFromImageFile(NetworkType network_type, QString ImagePath);

    bool insertFromImageFileWithJsonDetect(NetworkType network_type, QString ImageID, QString image_file_path, double min_x, double min_y, double max_x, double max_y);

    bool insertFromMaskJsonFileFolder(NetworkType network_type, QString mask_file_path, double min_x, double min_y, double max_x, double max_y);

    QJsonObject toClassificationMaskJsonObj(QString ImageID, double min_x, double min_y, double max_x, double max_y);

    QJsonObject toMaskJsonObj(NetworkType network_type, QString ImageID, double min_x, double min_y, double max_x, double max_y, int targetImageWidth, int targetImageHeight);

    bool toMaskJsonFile(NetworkType network_type, QString ImageID, QString mask_json_file_path, double min_x, double min_y, double max_x, double max_y, int targetImageWidth, int targetImageHeight);

    bool toMaskJsonFile(QJsonObject mask_json_Obj, QString mask_json_file_path);

private:
    void update_marked_and_unmarked_list();

    void loadFile();

    void create_work_station();

    void copy_unmarked_dataset();

    void init_mask_id_list();

    int get_mask_id(NetworkType network_type);

    void reuse_mask_id(NetworkType network_type, int mask_id);

    double getIOU(double x1, double y1, double x2, double y2, double new_x1, double new_y1, double new_x2, double new_y2);

    bool copyImage(NetworkType network_type, QString ImageID, QString mask_path, QString MaskID);

    bool copyImageWithMaskJson(NetworkType network_type, QString ImageID, QString mask_path, QString MaskID);

    bool copyImageWithTrainJson(NetworkType network_type, QString ImageID, QString mask_path, QString MaskID);

public:
    void init_param(QString root_path, QString target_path, int target_dataset_num, int workers_num);

    void create_mask_work_station();

    void recycle_mask_dataset();

    void create_train_dataset();

    void create_valid_dataset();

    void recycle_valid_dataset();

public slots:
    void slot_findFilesNum(int find_files_num);

    void slot_AddOpenFilePath(QString file_path);

    void slot_LoadDir_finished();

    void slot_CreateWorkStation();

    void slot_RecycleMask();

    void slot_CreateTrainDataset();

    void slot_CreateValidDataset();

    void slot_RecycleValid();

    void slot_InsertFolder(QString insert_folder_path);

signals:
    void signal_ChangeProgressMaxNum(int);

    void signal_ChangeProgressNum(int);

    void signal_MsgToShow(QString);

public:
    DatabaseManager* database_manager_;

    QString image_width_string_;
    QString image_height_string_;

    QMap<NetworkType, QString> network_type_translator_;
    QList<NetworkType> network_type_list_;

    QStandardItemModel* model_sourceimage_;
    QStandardItemModel* model_mask_Yolov3Region_;
    QStandardItemModel* model_mask_Yolov3Component_;
    QStandardItemModel* model_mask_ResNetClassification_;
    QStandardItemModel* model_predict_Yolov3Region_;
    QStandardItemModel* model_predict_Yolov3Component_;
    QStandardItemModel* model_predict_ResNetClassification_;

    NetworkType current_network_type_;

    bool connect_mask_;

private:
    QDir dir_;

    QImage image_;

    QList<QJsonObject> query_result_list_;

    QString root_path_;
    QString target_path_;
    int target_dataset_num_;
    int workers_num_;

    QVector<QString> marked_file_id_;
    QVector<QString> marked_file_path_;

    QVector<QString> unmarked_file_id_;
    QVector<QString> unmarked_file_path_;

    int dataset_num_for_each_worker_;
    int res_dataset_num_;
    QString work_path_;

    bool use_all_unmarked_dataset_;

    int debug_skip_num_;

    QList<QQueue<int>> mask_id_queue_list_;
    QList<int> new_mask_id_list_;

    int find_files_num_;

    QQueue<QString> file_path_queue_;
    int loaded_file_num_;
    int total_file_num_;
};

#endif // DATAMANAGER_H
