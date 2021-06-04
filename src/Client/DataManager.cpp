#include "DataManager.h"

#include <QDir>

DataManager::DataManager(QObject *parent) : QObject(parent)
{
    debug_skip_num_ = 1;

    if(false)
    {
        init();

        QString ImageID = "30464371786836306941455048485a57";

        outputAll(ImageID);

        //==========================================

        //insert img example
        insertSourceImage("test_out.jpg");

        outputAll(ImageID);

        //==========================================

        //revise img example
        QJsonObject img_revise_Obj;

        img_revise_Obj.insert("ImageID", ImageID);
        img_revise_Obj.insert("ImagePath", QDir::currentPath() + "/abababa");

        reviseSourceImage(img_revise_Obj);

        outputAll(ImageID);

        //==========================================

        //insert mask example
        QJsonObject mask_insert_Obj;

        mask_insert_Obj.insert("ImageID", ImageID);
        mask_insert_Obj.insert("ID", "0");
        mask_insert_Obj.insert("label", "test");
        mask_insert_Obj.insert("x1", "0.1");
        mask_insert_Obj.insert("x2", "0.2");
        mask_insert_Obj.insert("y1", "0.3");
        mask_insert_Obj.insert("y2", "0.4");

        insertMaskResult(Yolov3Component, mask_insert_Obj);

        outputAll(ImageID);

        //==========================================

        //insert predict example
        QJsonObject predict_insert_Obj;

        predict_insert_Obj.insert("ImageID", ImageID);
        predict_insert_Obj.insert("ID", "0");
        predict_insert_Obj.insert("label", "test");
        predict_insert_Obj.insert("x1", "0.1");
        predict_insert_Obj.insert("x2", "0.2");
        predict_insert_Obj.insert("y1", "0.3");
        predict_insert_Obj.insert("y2", "0.4");
        predict_insert_Obj.insert("score", "0.6");

        insertPredictResult(Yolov3Component, predict_insert_Obj);

        outputAll(ImageID);

        //==========================================

        //delete img example
        QJsonObject img_delete_Obj;

        img_delete_Obj.insert("ImageID", ImageID);

        deleteSourceImage(img_delete_Obj);

        outputAll(ImageID);

//        //==========================================

//        //delete mask example
//        QJsonObject mask_delete_Obj;

//        mask_delete_Obj.insert("ImageID", ImageID);
//        mask_delete_Obj.insert("ID", "0");

//        deleteMaskResult(Yolov3Component, mask_delete_Obj);

//        outputAll(ImageID);

//        //==========================================

//        //delete predict example
//        QJsonObject predict_delete_Obj;

//        predict_delete_Obj.insert("ImageID", ImageID);
//        predict_delete_Obj.insert("ID", "0");

//        deletePredictResult(Yolov3Component, predict_delete_Obj);

//        outputAll(ImageID);
    }
}

DataManager::~DataManager()
{

}

void DataManager::init(QString db_path)
{
    QString abs_db_path = db_path;

    if(abs_db_path !="")
    {
        if(!dir_.isAbsolutePath(abs_db_path))
        {
            abs_db_path = dir_.absoluteFilePath(abs_db_path);
        }

        if(abs_db_path[abs_db_path.size() - 1] != "/")
        {
            abs_db_path += "/";
        }
    }

    loaded_file_num_ = 0;
    total_file_num_ = 0;

    database_manager_ = new DatabaseManager(this, "QSQLITE", abs_db_path + "main_db", abs_db_path + "mainDb.db");

    loadConfig(QDir::currentPath() + "/Config/datamanager_config.json");

    network_type_translator_.insert(Yolov3Region, "Yolov3Region");
    network_type_translator_.insert(Yolov3Component, "Yolov3Component");
    network_type_translator_.insert(ResNetClassification, "ResNetClassification");

    network_type_list_.append(Yolov3Region);
    network_type_list_.append(Yolov3Component);
    network_type_list_.append(ResNetClassification);

    initAllModel();

    connect_mask_ = false;
}

void DataManager::initSourceImageModel()
{
    model_sourceimage_ = new QStandardItemModel();

    model_sourceimage_->setColumnCount(14);
    model_sourceimage_->setHeaderData(0, Qt::Horizontal, tr("ImageID"));
    model_sourceimage_->setHeaderData(1, Qt::Horizontal, tr("ImagePath"));
    model_sourceimage_->setHeaderData(2, Qt::Horizontal, tr("ImageWidth"));
    model_sourceimage_->setHeaderData(3, Qt::Horizontal, tr("ImageHeight"));
    model_sourceimage_->setHeaderData(4, Qt::Horizontal, tr("ImportTime"));
    model_sourceimage_->setHeaderData(5, Qt::Horizontal, tr("isYolov3RegionMasked"));
    model_sourceimage_->setHeaderData(6, Qt::Horizontal, tr("Yolov3RegionMaskID"));
    model_sourceimage_->setHeaderData(7, Qt::Horizontal, tr("isYolov3ComponentMasked"));
    model_sourceimage_->setHeaderData(8, Qt::Horizontal, tr("Yolov3ComponentMaskID"));
    model_sourceimage_->setHeaderData(9, Qt::Horizontal, tr("isResNetClassificationMasked"));
    model_sourceimage_->setHeaderData(10, Qt::Horizontal, tr("ResNetClassificationMaskID"));
    model_sourceimage_->setHeaderData(11, Qt::Horizontal, tr("isYolov3RegionPredicted"));
    model_sourceimage_->setHeaderData(12, Qt::Horizontal, tr("isYolov3ComponentPredicted"));
    model_sourceimage_->setHeaderData(13, Qt::Horizontal, tr("isResNetClassificationPredicted"));
}

void DataManager::initMaskModel()
{
    model_mask_Yolov3Region_ = new QStandardItemModel();

    model_mask_Yolov3Region_->setColumnCount(8);
    model_mask_Yolov3Region_->setHeaderData(0, Qt::Horizontal, tr("ImageID"));
    model_mask_Yolov3Region_->setHeaderData(1, Qt::Horizontal, tr("ID"));
    model_mask_Yolov3Region_->setHeaderData(2, Qt::Horizontal, tr("Time"));
    model_mask_Yolov3Region_->setHeaderData(3, Qt::Horizontal, tr("label"));
    model_mask_Yolov3Region_->setHeaderData(4, Qt::Horizontal, tr("x1"));
    model_mask_Yolov3Region_->setHeaderData(5, Qt::Horizontal, tr("y1"));
    model_mask_Yolov3Region_->setHeaderData(6, Qt::Horizontal, tr("x2"));
    model_mask_Yolov3Region_->setHeaderData(7, Qt::Horizontal, tr("y2"));

    model_mask_Yolov3Component_ = new QStandardItemModel();

    model_mask_Yolov3Component_->setColumnCount(8);
    model_mask_Yolov3Component_->setHeaderData(0, Qt::Horizontal, tr("ImageID"));
    model_mask_Yolov3Component_->setHeaderData(1, Qt::Horizontal, tr("ID"));
    model_mask_Yolov3Component_->setHeaderData(2, Qt::Horizontal, tr("Time"));
    model_mask_Yolov3Component_->setHeaderData(3, Qt::Horizontal, tr("label"));
    model_mask_Yolov3Component_->setHeaderData(4, Qt::Horizontal, tr("x1"));
    model_mask_Yolov3Component_->setHeaderData(5, Qt::Horizontal, tr("y1"));
    model_mask_Yolov3Component_->setHeaderData(6, Qt::Horizontal, tr("x2"));
    model_mask_Yolov3Component_->setHeaderData(7, Qt::Horizontal, tr("y2"));

    model_mask_ResNetClassification_ = new QStandardItemModel();

    model_mask_ResNetClassification_->setColumnCount(8);
    model_mask_ResNetClassification_->setHeaderData(0, Qt::Horizontal, tr("ImageID"));
    model_mask_ResNetClassification_->setHeaderData(1, Qt::Horizontal, tr("ID"));
    model_mask_ResNetClassification_->setHeaderData(2, Qt::Horizontal, tr("Time"));
    model_mask_ResNetClassification_->setHeaderData(3, Qt::Horizontal, tr("label"));
    model_mask_ResNetClassification_->setHeaderData(4, Qt::Horizontal, tr("x1"));
    model_mask_ResNetClassification_->setHeaderData(5, Qt::Horizontal, tr("y1"));
    model_mask_ResNetClassification_->setHeaderData(6, Qt::Horizontal, tr("x2"));
    model_mask_ResNetClassification_->setHeaderData(7, Qt::Horizontal, tr("y2"));
}

void DataManager::initPredictModel()
{
    model_predict_Yolov3Region_ = new QStandardItemModel();

    model_predict_Yolov3Region_->setColumnCount(9);
    model_predict_Yolov3Region_->setHeaderData(0, Qt::Horizontal, tr("ImageID"));
    model_predict_Yolov3Region_->setHeaderData(1, Qt::Horizontal, tr("ID"));
    model_predict_Yolov3Region_->setHeaderData(2, Qt::Horizontal, tr("Time"));
    model_predict_Yolov3Region_->setHeaderData(3, Qt::Horizontal, tr("label"));
    model_predict_Yolov3Region_->setHeaderData(4, Qt::Horizontal, tr("x1"));
    model_predict_Yolov3Region_->setHeaderData(5, Qt::Horizontal, tr("y1"));
    model_predict_Yolov3Region_->setHeaderData(6, Qt::Horizontal, tr("x2"));
    model_predict_Yolov3Region_->setHeaderData(7, Qt::Horizontal, tr("y2"));
    model_predict_Yolov3Region_->setHeaderData(8, Qt::Horizontal, tr("score"));

    model_predict_Yolov3Component_ = new QStandardItemModel();

    model_predict_Yolov3Component_->setColumnCount(9);
    model_predict_Yolov3Component_->setHeaderData(0, Qt::Horizontal, tr("ImageID"));
    model_predict_Yolov3Component_->setHeaderData(1, Qt::Horizontal, tr("ID"));
    model_predict_Yolov3Component_->setHeaderData(2, Qt::Horizontal, tr("Time"));
    model_predict_Yolov3Component_->setHeaderData(3, Qt::Horizontal, tr("label"));
    model_predict_Yolov3Component_->setHeaderData(4, Qt::Horizontal, tr("x1"));
    model_predict_Yolov3Component_->setHeaderData(5, Qt::Horizontal, tr("y1"));
    model_predict_Yolov3Component_->setHeaderData(6, Qt::Horizontal, tr("x2"));
    model_predict_Yolov3Component_->setHeaderData(7, Qt::Horizontal, tr("y2"));
    model_predict_Yolov3Component_->setHeaderData(8, Qt::Horizontal, tr("score"));

    model_predict_ResNetClassification_ = new QStandardItemModel();

    model_predict_ResNetClassification_->setColumnCount(9);
    model_predict_ResNetClassification_->setHeaderData(0, Qt::Horizontal, tr("ImageID"));
    model_predict_ResNetClassification_->setHeaderData(1, Qt::Horizontal, tr("ID"));
    model_predict_ResNetClassification_->setHeaderData(2, Qt::Horizontal, tr("Time"));
    model_predict_ResNetClassification_->setHeaderData(3, Qt::Horizontal, tr("label"));
    model_predict_ResNetClassification_->setHeaderData(4, Qt::Horizontal, tr("x1"));
    model_predict_ResNetClassification_->setHeaderData(5, Qt::Horizontal, tr("y1"));
    model_predict_ResNetClassification_->setHeaderData(6, Qt::Horizontal, tr("x2"));
    model_predict_ResNetClassification_->setHeaderData(7, Qt::Horizontal, tr("y2"));
    model_predict_ResNetClassification_->setHeaderData(8, Qt::Horizontal, tr("score"));
}

void DataManager::initAllModel()
{
    initSourceImageModel();

    initMaskModel();

    initPredictModel();
}

void DataManager::updateSourceImageModel()
{
    QJsonObject query_Obj;
    query_Obj.insert("ImageID", "null");
    query_Obj.insert("ImageIDOperator", "!=");

    querySourceImage(query_Obj);

    if(query_result_list_.size() == 0)
    {
        qDebug() << "DataManager::updateSourceImageModel : no image in db.";
        return;
    }

    model_sourceimage_->setRowCount(query_result_list_.size());

    for(int i = 0; i < query_result_list_.size(); ++i)
    {
        model_sourceimage_->setItem(i, 0, new QStandardItem(query_result_list_[i].value("ImageID").toString()));
        model_sourceimage_->setItem(i, 1, new QStandardItem(query_result_list_[i].value("ImagePath").toString()));
        model_sourceimage_->setItem(i, 2, new QStandardItem(query_result_list_[i].value("ImageWidth").toString()));
        model_sourceimage_->setItem(i, 3, new QStandardItem(query_result_list_[i].value("ImageHeight").toString()));
        model_sourceimage_->setItem(i, 4, new QStandardItem(query_result_list_[i].value("ImportTime").toString()));
        model_sourceimage_->setItem(i, 5, new QStandardItem(query_result_list_[i].value("isYolov3RegionMasked").toString()));
        model_sourceimage_->setItem(i, 6, new QStandardItem(query_result_list_[i].value("Yolov3RegionMaskID").toString()));
        model_sourceimage_->setItem(i, 7, new QStandardItem(query_result_list_[i].value("isYolov3ComponentMasked").toString()));
        model_sourceimage_->setItem(i, 8, new QStandardItem(query_result_list_[i].value("Yolov3ComponentMaskID").toString()));
        model_sourceimage_->setItem(i, 9, new QStandardItem(query_result_list_[i].value("isResNetClassificationMasked").toString()));
        model_sourceimage_->setItem(i, 10, new QStandardItem(query_result_list_[i].value("ResNetClassificationMaskID").toString()));
        model_sourceimage_->setItem(i, 11, new QStandardItem(query_result_list_[i].value("isYolov3RegionPredicted").toString()));
        model_sourceimage_->setItem(i, 12, new QStandardItem(query_result_list_[i].value("isYolov3ComponentPredicted").toString()));
        model_sourceimage_->setItem(i, 13, new QStandardItem(query_result_list_[i].value("isResNetClassificationPredicted").toString()));
    }
}

void DataManager::clearSourceImage()
{
    QJsonObject query_Obj;
    query_Obj.insert("ImageID", "null");
    query_Obj.insert("ImageIDOperator", "!=");

    querySourceImage(query_Obj);

    if(query_result_list_.size() == 0)
    {
        qDebug() << "DataManager::clearSourceImage : no data in db.";
        return;
    }

    for(int i = 0; i < query_result_list_.size(); ++i)
    {
        QString ImageID = query_result_list_[i].value("ImageID").toString();

        QJsonObject delete_Obj;
        delete_Obj.insert("ImageID", ImageID);

        deleteSourceImage(delete_Obj);
    }
}

void DataManager::insertToSourceImageModel(QString ImageID)
{
    QJsonObject query_Obj;
    query_Obj.insert("ImageID", ImageID);

    querySourceImage(query_Obj);

    if(query_result_list_.size() == 0)
    {
        qDebug() << "DataManager::insertToSourceImageModel : no image data in db.";
        return;
    }

    int current_row_num = model_sourceimage_->rowCount();

    model_sourceimage_->setRowCount(current_row_num + query_result_list_.size());

    for(int i = 0; i < query_result_list_.size(); ++i)
    {
        model_sourceimage_->setItem(current_row_num + i, 0, new QStandardItem(query_result_list_[i].value("ImageID").toString()));
        model_sourceimage_->setItem(current_row_num + i, 1, new QStandardItem(query_result_list_[i].value("ImagePath").toString()));
        model_sourceimage_->setItem(current_row_num + i, 2, new QStandardItem(query_result_list_[i].value("ImageWidth").toString()));
        model_sourceimage_->setItem(current_row_num + i, 3, new QStandardItem(query_result_list_[i].value("ImageHeight").toString()));
        model_sourceimage_->setItem(current_row_num + i, 4, new QStandardItem(query_result_list_[i].value("ImportTime").toString()));
        model_sourceimage_->setItem(current_row_num + i, 5, new QStandardItem(query_result_list_[i].value("isYolov3RegionMasked").toString()));
        model_sourceimage_->setItem(current_row_num + i, 6, new QStandardItem(query_result_list_[i].value("Yolov3RegionMaskID").toString()));
        model_sourceimage_->setItem(current_row_num + i, 7, new QStandardItem(query_result_list_[i].value("isYolov3ComponentMasked").toString()));
        model_sourceimage_->setItem(current_row_num + i, 8, new QStandardItem(query_result_list_[i].value("Yolov3ComponentMaskID").toString()));
        model_sourceimage_->setItem(current_row_num + i, 9, new QStandardItem(query_result_list_[i].value("isResNetClassificationMasked").toString()));
        model_sourceimage_->setItem(current_row_num + i, 10, new QStandardItem(query_result_list_[i].value("ResNetClassificationMaskID").toString()));
        model_sourceimage_->setItem(current_row_num + i, 11, new QStandardItem(query_result_list_[i].value("isYolov3RegionPredicted").toString()));
        model_sourceimage_->setItem(current_row_num + i, 12, new QStandardItem(query_result_list_[i].value("isYolov3ComponentPredicted").toString()));
        model_sourceimage_->setItem(current_row_num + i, 13, new QStandardItem(query_result_list_[i].value("isResNetClassificationPredicted").toString()));
    }
}

void DataManager::updateMaskModel(QString ImageID)
{
    QJsonObject query_Obj;
    query_Obj["ImageID"] = ImageID;

    queryMaskResult(Yolov3Region, query_Obj);

    model_mask_Yolov3Region_->setRowCount(query_result_list_.size());

    if(query_result_list_.size() > 0)
    {
        for(int i = 0; i < query_result_list_.size(); ++i)
        {
            model_mask_Yolov3Region_->setItem(i, 0, new QStandardItem(query_result_list_[i].value("ImageID").toString()));
            model_mask_Yolov3Region_->setItem(i, 1, new QStandardItem(query_result_list_[i].value("ID").toString()));
            model_mask_Yolov3Region_->setItem(i, 2, new QStandardItem(query_result_list_[i].value("Time").toString()));
            model_mask_Yolov3Region_->setItem(i, 3, new QStandardItem(query_result_list_[i].value("label").toString()));
            model_mask_Yolov3Region_->setItem(i, 4, new QStandardItem(query_result_list_[i].value("x1").toString()));
            model_mask_Yolov3Region_->setItem(i, 5, new QStandardItem(query_result_list_[i].value("y1").toString()));
            model_mask_Yolov3Region_->setItem(i, 6, new QStandardItem(query_result_list_[i].value("x2").toString()));
            model_mask_Yolov3Region_->setItem(i, 7, new QStandardItem(query_result_list_[i].value("y2").toString()));
        }
    }

    queryMaskResult(Yolov3Component, query_Obj);

    model_mask_Yolov3Component_->setRowCount(query_result_list_.size());

    if(query_result_list_.size() > 0)
    {
        for(int i = 0; i < query_result_list_.size(); ++i)
        {
            model_mask_Yolov3Component_->setItem(i, 0, new QStandardItem(query_result_list_[i].value("ImageID").toString()));
            model_mask_Yolov3Component_->setItem(i, 1, new QStandardItem(query_result_list_[i].value("ID").toString()));
            model_mask_Yolov3Component_->setItem(i, 2, new QStandardItem(query_result_list_[i].value("Time").toString()));
            model_mask_Yolov3Component_->setItem(i, 3, new QStandardItem(query_result_list_[i].value("label").toString()));
            model_mask_Yolov3Component_->setItem(i, 4, new QStandardItem(query_result_list_[i].value("x1").toString()));
            model_mask_Yolov3Component_->setItem(i, 5, new QStandardItem(query_result_list_[i].value("y1").toString()));
            model_mask_Yolov3Component_->setItem(i, 6, new QStandardItem(query_result_list_[i].value("x2").toString()));
            model_mask_Yolov3Component_->setItem(i, 7, new QStandardItem(query_result_list_[i].value("y2").toString()));
        }
    }

    queryMaskResult(ResNetClassification, query_Obj);

    model_mask_ResNetClassification_->setRowCount(query_result_list_.size());

    if(query_result_list_.size() > 0)
    {
        for(int i = 0; i < query_result_list_.size(); ++i)
        {
            model_mask_ResNetClassification_->setItem(i, 0, new QStandardItem(query_result_list_[i].value("ImageID").toString()));
            model_mask_ResNetClassification_->setItem(i, 1, new QStandardItem(query_result_list_[i].value("ID").toString()));
            model_mask_ResNetClassification_->setItem(i, 2, new QStandardItem(query_result_list_[i].value("Time").toString()));
            model_mask_ResNetClassification_->setItem(i, 3, new QStandardItem(query_result_list_[i].value("label").toString()));
            model_mask_ResNetClassification_->setItem(i, 4, new QStandardItem(query_result_list_[i].value("x1").toString()));
            model_mask_ResNetClassification_->setItem(i, 5, new QStandardItem(query_result_list_[i].value("y1").toString()));
            model_mask_ResNetClassification_->setItem(i, 6, new QStandardItem(query_result_list_[i].value("x2").toString()));
            model_mask_ResNetClassification_->setItem(i, 7, new QStandardItem(query_result_list_[i].value("y2").toString()));
        }
    }
}

void DataManager::updatePredictModel(QString ImageID)
{
    QJsonObject query_Obj;
    query_Obj["ImageID"] = ImageID;

    queryPredictResult(Yolov3Region, query_Obj);

    model_predict_Yolov3Region_->setRowCount(query_result_list_.size());

    if(query_result_list_.size() > 0)
    {
        for(int i = 0; i < query_result_list_.size(); ++i)
        {
            model_predict_Yolov3Region_->setItem(i, 0, new QStandardItem(query_result_list_[i].value("ImageID").toString()));
            model_predict_Yolov3Region_->setItem(i, 1, new QStandardItem(query_result_list_[i].value("ID").toString()));
            model_predict_Yolov3Region_->setItem(i, 2, new QStandardItem(query_result_list_[i].value("Time").toString()));
            model_predict_Yolov3Region_->setItem(i, 3, new QStandardItem(query_result_list_[i].value("label").toString()));
            model_predict_Yolov3Region_->setItem(i, 4, new QStandardItem(query_result_list_[i].value("x1").toString()));
            model_predict_Yolov3Region_->setItem(i, 5, new QStandardItem(query_result_list_[i].value("y1").toString()));
            model_predict_Yolov3Region_->setItem(i, 6, new QStandardItem(query_result_list_[i].value("x2").toString()));
            model_predict_Yolov3Region_->setItem(i, 7, new QStandardItem(query_result_list_[i].value("y2").toString()));
            model_predict_Yolov3Region_->setItem(i, 8, new QStandardItem(query_result_list_[i].value("score").toString()));
        }
    }

    queryPredictResult(Yolov3Component, query_Obj);

    model_predict_Yolov3Component_->setRowCount(query_result_list_.size());

    if(query_result_list_.size() > 0)
    {
        for(int i = 0; i < query_result_list_.size(); ++i)
        {
            model_predict_Yolov3Component_->setItem(i, 0, new QStandardItem(query_result_list_[i].value("ImageID").toString()));
            model_predict_Yolov3Component_->setItem(i, 1, new QStandardItem(query_result_list_[i].value("ID").toString()));
            model_predict_Yolov3Component_->setItem(i, 2, new QStandardItem(query_result_list_[i].value("Time").toString()));
            model_predict_Yolov3Component_->setItem(i, 3, new QStandardItem(query_result_list_[i].value("label").toString()));
            model_predict_Yolov3Component_->setItem(i, 4, new QStandardItem(query_result_list_[i].value("x1").toString()));
            model_predict_Yolov3Component_->setItem(i, 5, new QStandardItem(query_result_list_[i].value("y1").toString()));
            model_predict_Yolov3Component_->setItem(i, 6, new QStandardItem(query_result_list_[i].value("x2").toString()));
            model_predict_Yolov3Component_->setItem(i, 7, new QStandardItem(query_result_list_[i].value("y2").toString()));
            model_predict_Yolov3Component_->setItem(i, 8, new QStandardItem(query_result_list_[i].value("score").toString()));
        }
    }

    queryPredictResult(ResNetClassification, query_Obj);

    model_predict_ResNetClassification_->setRowCount(query_result_list_.size());

    if(query_result_list_.size() > 0)
    {
        for(int i = 0; i < query_result_list_.size(); ++i)
        {
            model_predict_ResNetClassification_->setItem(i, 0, new QStandardItem(query_result_list_[i].value("ImageID").toString()));
            model_predict_ResNetClassification_->setItem(i, 1, new QStandardItem(query_result_list_[i].value("ID").toString()));
            model_predict_ResNetClassification_->setItem(i, 2, new QStandardItem(query_result_list_[i].value("Time").toString()));
            model_predict_ResNetClassification_->setItem(i, 3, new QStandardItem(query_result_list_[i].value("label").toString()));
            model_predict_ResNetClassification_->setItem(i, 4, new QStandardItem(query_result_list_[i].value("x1").toString()));
            model_predict_ResNetClassification_->setItem(i, 5, new QStandardItem(query_result_list_[i].value("y1").toString()));
            model_predict_ResNetClassification_->setItem(i, 6, new QStandardItem(query_result_list_[i].value("x2").toString()));
            model_predict_ResNetClassification_->setItem(i, 7, new QStandardItem(query_result_list_[i].value("y2").toString()));
            model_predict_ResNetClassification_->setItem(i, 8, new QStandardItem(query_result_list_[i].value("score").toString()));
        }
    }
}

void DataManager::updateMaskAndPredictModel(QString ImageID)
{
    updateMaskModel(ImageID);

    updatePredictModel(ImageID);
}

void DataManager::updateMaskAndPredictModel(int row_index)
{
    if(row_index < 0 || row_index >= model_sourceimage_->columnCount())
    {
        qDebug() << "DataManager::updateMaskAndPredictModel : row input out of range.";
        return;
    }

    QString ImageID = model_sourceimage_->item(row_index, 0)->text();

    updateMaskAndPredictModel(ImageID);
}

void DataManager::outputSourceImage(QString ImageID)
{
    QJsonObject search_Obj;

    search_Obj.insert("ImageID", ImageID);

    querySourceImage(search_Obj);

    qDebug() << "====SourceImage====";
    if(query_result_list_.size() == 0)
    {
        qDebug() << "NULL...";
        return;
    }

    for(int i = 0; i < query_result_list_.size(); ++i)
    {
        qDebug() << "ImageID : " << query_result_list_[i].value("ImageID").toString();
        qDebug() << "ImagePath : " << query_result_list_[i].value("ImagePath").toString();
        qDebug() << "ImageWidth : " << query_result_list_[i].value("ImageWidth").toString();
        qDebug() << "ImageHeight : " << query_result_list_[i].value("ImageHeight").toString();
        qDebug() << "ImportTime : " << query_result_list_[i].value("ImportTime").toString();
        qDebug() << "isYolov3RegionMasked : " << query_result_list_[i].value("isYolov3RegionMasked").toString();
        qDebug() << "isYolov3ComponentMasked : " << query_result_list_[i].value("isYolov3ComponentMasked").toString();
        qDebug() << "isResNetClassificationMasked : " << query_result_list_[i].value("isResNetClassificationMasked").toString();
        qDebug() << "isYolov3RegionPredicted : " << query_result_list_[i].value("isYolov3RegionPredicted").toString();
        qDebug() << "isYolov3ComponentPredicted : " << query_result_list_[i].value("isYolov3ComponentPredicted").toString();
        qDebug() << "isResNetClassificationPredicted : " << query_result_list_[i].value("isResNetClassificationPredicted").toString();
    }
}

void DataManager::outputMaskBoundingBox(QString ImageID)
{
    QJsonObject search_Obj;

    search_Obj.insert("ImageID", ImageID);

    qDebug() << "====MaskBoundingBox====";
    for(int i = 0; i < network_type_translator_.size(); ++i)
    {
        queryMaskResult(network_type_list_[i], search_Obj);

        qDebug() << "========" << network_type_translator_[network_type_list_[i]] << "========";

        if(query_result_list_.size() == 0)
        {
            qDebug() << "NULL...";
            continue;
        }

        for(int i = 0; i < query_result_list_.size(); ++i)
        {
            qDebug() << "ImageID : " << query_result_list_[i].value("ImageID").toString();
            qDebug() << "ID : " << query_result_list_[i].value("ID").toString();
            qDebug() << "label : " << query_result_list_[i].value("label").toString();
            qDebug() << "x1 : " << query_result_list_[i].value("x1").toString();
            qDebug() << "x2 : " << query_result_list_[i].value("x2").toString();
            qDebug() << "y1 : " << query_result_list_[i].value("y1").toString();
            qDebug() << "y2 : " << query_result_list_[i].value("y2").toString();
        }
    }
}

void DataManager::outputPredictBoundingBox(QString ImageID)
{
    QJsonObject search_Obj;

    search_Obj.insert("ImageID", ImageID);

    qDebug() << "====PredictBoundingBox====";
    for(int i = 0; i < network_type_translator_.size(); ++i)
    {
        queryPredictResult(network_type_list_[i], search_Obj);

        qDebug() << "========" << network_type_translator_[network_type_list_[i]] << "========";

        if(query_result_list_.size() == 0)
        {
            qDebug() << "NULL...";
            continue;
        }

        for(int i = 0; i < query_result_list_.size(); ++i)
        {
            qDebug() << "ImageID : " << query_result_list_[i].value("ImageID").toString();
            qDebug() << "ID : " << query_result_list_[i].value("ID").toString();
            qDebug() << "Time : " << query_result_list_[i].value("Time").toString();
            qDebug() << "label : " << query_result_list_[i].value("label").toString();
            qDebug() << "x1 : " << query_result_list_[i].value("x1").toString();
            qDebug() << "x2 : " << query_result_list_[i].value("x2").toString();
            qDebug() << "y1 : " << query_result_list_[i].value("y1").toString();
            qDebug() << "y2 : " << query_result_list_[i].value("y2").toString();
            qDebug() << "score : " << query_result_list_[i].value("score").toString();
        }
    }
}

void DataManager::outputAll(QString ImageID)
{
    outputSourceImage(ImageID);

    outputMaskBoundingBox(ImageID);

    outputPredictBoundingBox(ImageID);
}

void DataManager::loadConfig(QString filename)
{
    QFile file(filename);
    if(!file.open(QFile::ReadOnly))
    {
        qDebug() << "DataManager::loadConfig : open file failed.";

        return;
    }

    QJsonParseError err;
    QJsonDocument doc=QJsonDocument::fromJson(file.readAll(),&err);

    if(err.error!=QJsonParseError::NoError){
        qDebug()<<"parse error:"<<err.errorString();
        return;
    }

    QJsonObject root_config_obj = doc.object();

    if(!root_config_obj.contains("DatabaseConfig"))
    {
        qDebug() << "DataManager::loadConfig : load config failed.";
        return;
    }

    database_manager_->loadConfig(root_config_obj.value("DatabaseConfig").toObject());

    database_manager_->initDatabaseFromConfig();
}

QJsonObject DataManager::sourceImageJson(QString ImageID,
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
                                         QString isResNetClassificationPredicted)
{
    QJsonObject source_image_Obj;

    source_image_Obj.insert("ImageID", ImageID);
    source_image_Obj.insert("ImagePath", ImagePath);
    source_image_Obj.insert("ImageWidth", ImageWidth);
    source_image_Obj.insert("ImageHeight", ImageHeight);
    source_image_Obj.insert("ImportTime", ImportTime);
    source_image_Obj.insert("isYolov3RegionMasked", isYolov3RegionMasked);
    source_image_Obj.insert("Yolov3RegionMaskID", Yolov3RegionMaskID);
    source_image_Obj.insert("isYolov3ComponentMasked", isYolov3ComponentMasked);
    source_image_Obj.insert("Yolov3ComponentMaskID", Yolov3ComponentMaskID);
    source_image_Obj.insert("isResNetClassificationMasked", isResNetClassificationMasked);
    source_image_Obj.insert("ResNetClassificationMaskID", ResNetClassificationMaskID);
    source_image_Obj.insert("isYolov3RegionPredicted", isYolov3RegionPredicted);
    source_image_Obj.insert("isYolov3ComponentPredicted", isYolov3ComponentPredicted);
    source_image_Obj.insert("isResNetClassificationPredicted", isResNetClassificationPredicted);

    return source_image_Obj;
}

QJsonObject DataManager::sourceImageWithOperatorJson(QString ImageIDOperator,
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
                                                     QString isResNetClassificationPredicted)
{
    QJsonObject source_image_with_operator_Obj;

    source_image_with_operator_Obj.insert("ImageIDOperator", ImageIDOperator);
    source_image_with_operator_Obj.insert("ImageID", ImageID);
    source_image_with_operator_Obj.insert("ImagePathOperator", ImagePathOperator);
    source_image_with_operator_Obj.insert("ImagePath", ImagePath);
    source_image_with_operator_Obj.insert("ImageWidthOperator", ImageWidthOperator);
    source_image_with_operator_Obj.insert("ImageWidth", ImageWidth);
    source_image_with_operator_Obj.insert("ImageHeightOperator", ImageHeightOperator);
    source_image_with_operator_Obj.insert("ImageHeight", ImageHeight);
    source_image_with_operator_Obj.insert("ImportTimeOperator", ImportTimeOperator);
    source_image_with_operator_Obj.insert("ImportTime", ImportTime);
    source_image_with_operator_Obj.insert("isYolov3RegionMaskedOperator", isYolov3RegionMaskedOperator);
    source_image_with_operator_Obj.insert("isYolov3RegionMasked", isYolov3RegionMasked);
    source_image_with_operator_Obj.insert("Yolov3RegionMaskIDOperator", Yolov3RegionMaskIDOperator);
    source_image_with_operator_Obj.insert("Yolov3RegionMaskID", Yolov3RegionMaskID);
    source_image_with_operator_Obj.insert("isYolov3ComponentMaskedOperator", isYolov3ComponentMaskedOperator);
    source_image_with_operator_Obj.insert("isYolov3ComponentMasked", isYolov3ComponentMasked);
    source_image_with_operator_Obj.insert("Yolov3ComponentMaskIDOperator", Yolov3ComponentMaskIDOperator);
    source_image_with_operator_Obj.insert("Yolov3ComponentMaskID", Yolov3ComponentMaskID);
    source_image_with_operator_Obj.insert("isResNetClassificationMaskedOperator", isResNetClassificationMaskedOperator);
    source_image_with_operator_Obj.insert("isResNetClassificationMasked", isResNetClassificationMasked);
    source_image_with_operator_Obj.insert("ResNetClassificationMaskIDOperator", ResNetClassificationMaskIDOperator);
    source_image_with_operator_Obj.insert("ResNetClassificationMaskID", ResNetClassificationMaskID);
    source_image_with_operator_Obj.insert("isYolov3RegionPredictedOperator", isYolov3RegionPredictedOperator);
    source_image_with_operator_Obj.insert("isYolov3RegionPredicted", isYolov3RegionPredicted);
    source_image_with_operator_Obj.insert("isYolov3ComponentPredictedOperator", isYolov3ComponentPredictedOperator);
    source_image_with_operator_Obj.insert("isYolov3ComponentPredicted", isYolov3ComponentPredicted);
    source_image_with_operator_Obj.insert("isResNetClassificationPredictedOperator", isResNetClassificationPredictedOperator);
    source_image_with_operator_Obj.insert("isResNetClassificationPredicted", isResNetClassificationPredicted);

    return source_image_with_operator_Obj;
}

QJsonObject DataManager::maskBoundingBoxJson(QString ImageID,
                                             QString ID,
                                             QString Time,
                                             QString label,
                                             QString x1,
                                             QString x2,
                                             QString y1,
                                             QString y2)
{
    QJsonObject bounding_box_Obj;

    bounding_box_Obj.insert("ImageID", ImageID);
    bounding_box_Obj.insert("ID", ID);
    bounding_box_Obj.insert("Time", Time);
    bounding_box_Obj.insert("label", label);
    bounding_box_Obj.insert("x1", x1);
    bounding_box_Obj.insert("x2", x2);
    bounding_box_Obj.insert("y1", y1);
    bounding_box_Obj.insert("y2", y2);

    return bounding_box_Obj;
}

QJsonObject DataManager::maskBoundingBoxWithOperatorJson(QString ImageIDOperator,
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
                                                         QString y2)
{
    QJsonObject bounding_box_with_operator_Obj;

    bounding_box_with_operator_Obj.insert("ImageIDOperator", ImageIDOperator);
    bounding_box_with_operator_Obj.insert("ImageID", ImageID);
    bounding_box_with_operator_Obj.insert("IDOperator", IDOperator);
    bounding_box_with_operator_Obj.insert("ID", ID);
    bounding_box_with_operator_Obj.insert("TimeOperator", TimeOperator);
    bounding_box_with_operator_Obj.insert("Time", Time);
    bounding_box_with_operator_Obj.insert("labelOperator", labelOperator);
    bounding_box_with_operator_Obj.insert("label", label);
    bounding_box_with_operator_Obj.insert("x1Operator", x1Operator);
    bounding_box_with_operator_Obj.insert("x1", x1);
    bounding_box_with_operator_Obj.insert("x2Operator", x2Operator);
    bounding_box_with_operator_Obj.insert("x2", x2);
    bounding_box_with_operator_Obj.insert("y1Operator", y1Operator);
    bounding_box_with_operator_Obj.insert("y1", y1);
    bounding_box_with_operator_Obj.insert("y2Operator", y2Operator);
    bounding_box_with_operator_Obj.insert("y2", y2);

    return bounding_box_with_operator_Obj;
}

QJsonObject DataManager::predictBoundingBoxJson(QString ImageID,
                                                QString ID,
                                                QString Time,
                                                QString label,
                                                QString x1,
                                                QString x2,
                                                QString y1,
                                                QString y2,
                                                QString score)
{
    QJsonObject bounding_box_Obj;

    bounding_box_Obj.insert("ImageID", ImageID);
    bounding_box_Obj.insert("ID", ID);
    bounding_box_Obj.insert("Time", Time);
    bounding_box_Obj.insert("label", label);
    bounding_box_Obj.insert("x1", x1);
    bounding_box_Obj.insert("x2", x2);
    bounding_box_Obj.insert("y1", y1);
    bounding_box_Obj.insert("y2", y2);
    bounding_box_Obj.insert("score", score);

    return bounding_box_Obj;
}

QJsonObject DataManager::predictBoundingBoxWithOperatorJson(QString ImageIDOperator,
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
                                                            QString score)
{
    QJsonObject bounding_box_with_operator_Obj;

    bounding_box_with_operator_Obj.insert("ImageIDOperator", ImageIDOperator);
    bounding_box_with_operator_Obj.insert("ImageID", ImageID);
    bounding_box_with_operator_Obj.insert("IDOperator", IDOperator);
    bounding_box_with_operator_Obj.insert("ID", ID);
    bounding_box_with_operator_Obj.insert("TimeOperator", TimeOperator);
    bounding_box_with_operator_Obj.insert("Time", Time);
    bounding_box_with_operator_Obj.insert("labelOperator", labelOperator);
    bounding_box_with_operator_Obj.insert("label", label);
    bounding_box_with_operator_Obj.insert("x1Operator", x1Operator);
    bounding_box_with_operator_Obj.insert("x1", x1);
    bounding_box_with_operator_Obj.insert("x2Operator", x2Operator);
    bounding_box_with_operator_Obj.insert("x2", x2);
    bounding_box_with_operator_Obj.insert("y1Operator", y1Operator);
    bounding_box_with_operator_Obj.insert("y1", y1);
    bounding_box_with_operator_Obj.insert("y2Operator", y2Operator);
    bounding_box_with_operator_Obj.insert("y2", y2);
    bounding_box_with_operator_Obj.insert("scoreOperator", scoreOperator);
    bounding_box_with_operator_Obj.insert("score", score);

    return bounding_box_with_operator_Obj;
}

QMap<QString, QString> DataManager::jsonToMap(QJsonObject json_Obj)
{
    QMap<QString, QString> json_map;

    QString keyString;
    QString valueString;

    QJsonObject::Iterator it;

    for(it = json_Obj.begin(); it != json_Obj.end(); ++it)
    {
        valueString = it.value().toString();

        if(valueString != "")
        {
            keyString = it.key();

            json_map.insert(keyString, valueString);
        }
    }

    return json_map;
}

QList<QMap<QString, QString>> DataManager::jsonToMapList(QJsonObject json_Obj)
{
    QList<QMap<QString, QString>> json_map_list;

    QStringList key_list;

    QString keyString;
    QString valueString;
    QString operatorValueString;

    QJsonObject::Iterator it;

    for(it = json_Obj.begin(); it != json_Obj.end(); ++it)
    {
        valueString = it.value().toString();

        if(valueString != "")
        {
            keyString = it.key();

            if(!keyString.contains("Operator"))
            {
                operatorValueString = json_Obj.find(keyString + "Operator").value().toString();

                if(operatorValueString == "")
                {
                    operatorValueString = "==";
                }

                QMap<QString, QString> json_map;

                json_map.insert("field", keyString);
                json_map.insert("operator", operatorValueString);
                json_map.insert("value", valueString);

                json_map_list.append(json_map);
            }
        }
    }

    return json_map_list;
}

bool DataManager::insertSourceImage(QJsonObject insert_Obj)
{
    QMap<QString, QString> insert_map = jsonToMap(insert_Obj);

    if(!insert_map.keys().contains("ImageID"))
    {
        qDebug() << "DataManager::insertSourceImage : input doesn't have ImageID.";
        return false;
    }

    QJsonObject query_Obj;
    query_Obj.insert("ImageID", insert_map["ImageID"]);

    querySourceImage(query_Obj);

    if(query_result_list_.size() > 0)
    {
//        qDebug() << "DataManager::insertSourceImage : you already have this image.";
        return false;
    }

    if(!insert_map.keys().contains("ImportTime"))
    {
        QString current_Time = QDateTime::currentDateTime().toString("yy/MM/dd hh:mm:ss");

        insert_map.insert("ImportTime", current_Time);
    }

    return database_manager_->insertRecord("SourceImage", insert_map);
}

bool DataManager::insertSourceImage(QString ImagePath)
{
    QString absImagePath = ImagePath;
    if(!dir_.isAbsolutePath(ImagePath))
    {
        absImagePath = dir_.absoluteFilePath(absImagePath);
    }

    QString current_date_time = QDateTime::currentDateTime().toString("yy/MM/dd hh:mm:ss");

    if(!dir_.exists(absImagePath))
    {
        qDebug() << "DataManager::insertSourceImage : " + absImagePath + " not exist.";
        return false;
    }

    QString current_file_hash = getFileHash(absImagePath);

    if(current_file_hash == "")
    {
        qDebug() << "DataManager::insertSourceImage : " + absImagePath + " open failed.";
        return false;
    }

//    QJsonObject query_Obj;
//    query_Obj.insert("ImageID", current_file_hash);

//    querySourceImage(query_Obj);

//    if(query_result_list_.size() > 0)
//    {
//        qDebug() << "DataManager::insertSourceImage : source image exists in db.";
//        return false;
//    }

    QJsonObject insert_Obj = sourceImageJson(current_file_hash,
                                             absImagePath,
                                             image_width_string_,
                                             image_height_string_,
                                             current_date_time,
                                             "0",
                                             "-1",
                                             "0",
                                             "-1",
                                             "0",
                                             "-1",
                                             "0",
                                             "0",
                                             "0");

    return insertSourceImage(insert_Obj);
}

bool DataManager::reviseSourceImage(QString ImageID,
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
                                    QString isResNetClassificationPredicted)
{
    QJsonObject revise_Obj = sourceImageJson(ImageID,
                                             ImagePath,
                                             ImageWidth,
                                             ImageHeight,
                                             ImportTime,
                                             isYolov3RegionMasked,
                                             Yolov3RegionMaskID,
                                             isYolov3ComponentMasked,
                                             Yolov3ComponentMaskID,
                                             isResNetClassificationMasked,
                                             ResNetClassificationMaskID,
                                             isYolov3RegionPredicted,
                                             isYolov3ComponentPredicted,
                                             isResNetClassificationPredicted);

    return reviseSourceImage(revise_Obj);
}

bool DataManager::reviseSourceImage(QJsonObject revise_Obj)
{
    QMap<QString, QString> revise_map = jsonToMap(revise_Obj);

    if(revise_map.find("ImageID").value() == "")
    {
        qDebug() << "DataManager::reviseSourceImage : input doesn't have ImageID.";
        return false;
    }
    else if(revise_map.size() == 1)
    {
        qDebug() << "DataManager::reviseSourceImage : no need to revise.";
        return true;
    }

    return database_manager_->reviseRecord("SourceImage", "ImageID", revise_map);
}

bool DataManager::querySourceImage(QString ImageIDOperator,
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
                                   QString isResNetClassificationPredicted)
{
    QJsonObject query_Obj = sourceImageWithOperatorJson(ImageIDOperator,
                                                        ImageID,
                                                        ImagePathOperator,
                                                        ImagePath,
                                                        ImageWidthOperator,
                                                        ImageWidth,
                                                        ImageHeightOperator,
                                                        ImageHeight,
                                                        ImportTimeOperator,
                                                        ImportTime,
                                                        isYolov3RegionMaskedOperator,
                                                        isYolov3RegionMasked,
                                                        Yolov3RegionMaskIDOperator,
                                                        Yolov3RegionMaskID,
                                                        isYolov3ComponentMaskedOperator,
                                                        isYolov3ComponentMasked,
                                                        Yolov3ComponentMaskIDOperator,
                                                        Yolov3ComponentMaskID,
                                                        isResNetClassificationMaskedOperator,
                                                        isResNetClassificationMasked,
                                                        ResNetClassificationMaskIDOperator,
                                                        ResNetClassificationMaskID,
                                                        isYolov3RegionPredictedOperator,
                                                        isYolov3RegionPredicted,
                                                        isYolov3ComponentPredictedOperator,
                                                        isYolov3ComponentPredicted,
                                                        isResNetClassificationPredictedOperator,
                                                        isResNetClassificationPredicted);

    return querySourceImage(query_Obj);
}

bool DataManager::querySourceImage(QJsonObject query_Obj)
{
    QList<QMap<QString, QString>> query_map_list = jsonToMapList(query_Obj);

    if(query_map_list.size() == 0)
    {
        qDebug() << "DataManager::querySourceImage : empty input.";
        return false;
    }

    query_result_list_.clear();

    if(!database_manager_->queryRecord("SourceImage", query_map_list))
    {
        return false;
    }

    while(database_manager_->query_->next())
    {
        QString ImageID = database_manager_->query_->value("ImageID").toString();
        QString ImagePath = database_manager_->query_->value("ImagePath").toString();
        QString ImageWidth = database_manager_->query_->value("ImageWidth").toString();
        QString ImageHeight = database_manager_->query_->value("ImageHeight").toString();
        QString ImportTime = database_manager_->query_->value("ImportTime").toString();
        QString isYolov3RegionMasked = database_manager_->query_->value("isYolov3RegionMasked").toString();
        QString Yolov3RegionMaskID = database_manager_->query_->value("Yolov3RegionMaskID").toString();
        QString isYolov3ComponentMasked = database_manager_->query_->value("isYolov3ComponentMasked").toString();
        QString Yolov3ComponentMaskID = database_manager_->query_->value("Yolov3ComponentMaskID").toString();
        QString isResNetClassificationMasked = database_manager_->query_->value("isResNetClassificationMasked").toString();
        QString ResNetClassificationMaskID = database_manager_->query_->value("ResNetClassificationMaskID").toString();
        QString isYolov3RegionPredicted = database_manager_->query_->value("isYolov3RegionPredicted").toString();
        QString isYolov3ComponentPredicted = database_manager_->query_->value("isYolov3ComponentPredicted").toString();
        QString isResNetClassificationPredicted = database_manager_->query_->value("isResNetClassificationPredicted").toString();

        QJsonObject query_save_Obj = sourceImageJson(ImageID,
                                                     ImagePath,
                                                     ImageWidth,
                                                     ImageHeight,
                                                     ImportTime,
                                                     isYolov3RegionMasked,
                                                     Yolov3RegionMaskID,
                                                     isYolov3ComponentMasked,
                                                     Yolov3ComponentMaskID,
                                                     isResNetClassificationMasked,
                                                     ResNetClassificationMaskID,
                                                     isYolov3RegionPredicted,
                                                     isYolov3ComponentPredicted,
                                                     isResNetClassificationPredicted);

        query_result_list_.append(query_save_Obj);
    }

    return true;
}

bool DataManager::deleteSourceImage(QString ImageIDOperator,
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
                                    QString isResNetClassificationPredicted)
{
    QJsonObject delete_Obj = sourceImageWithOperatorJson(ImageIDOperator,
                                                         ImageID,
                                                         ImagePathOperator,
                                                         ImagePath,
                                                         ImageWidthOperator,
                                                         ImageWidth,
                                                         ImageHeightOperator,
                                                         ImageHeight,
                                                         ImportTimeOperator,
                                                         ImportTime,
                                                         isYolov3RegionMaskedOperator,
                                                         isYolov3RegionMasked,
                                                         Yolov3RegionMaskIDOperator,
                                                         Yolov3RegionMaskID,
                                                         isYolov3ComponentMaskedOperator,
                                                         isYolov3ComponentMasked,
                                                         Yolov3ComponentMaskIDOperator,
                                                         Yolov3ComponentMaskID,
                                                         isResNetClassificationMaskedOperator,
                                                         isResNetClassificationMasked,
                                                         ResNetClassificationMaskIDOperator,
                                                         ResNetClassificationMaskID,
                                                         isYolov3RegionPredictedOperator,
                                                         isYolov3RegionPredicted,
                                                         isYolov3ComponentPredictedOperator,
                                                         isYolov3ComponentPredicted,
                                                         isResNetClassificationPredictedOperator,
                                                         isResNetClassificationPredicted);

    return deleteSourceImage(delete_Obj);
}

bool DataManager::deleteSourceImage(QJsonObject delete_Obj)
{
    QList<QMap<QString, QString>> delete_map_list = jsonToMapList(delete_Obj);

    if(delete_map_list.size() == 0)
    {
        qDebug() << "DataManager::deleteSourceImage : empty input.";
        return false;
    }

    QString ImageID;

    for(int i = 0; i < delete_map_list.size(); ++i)
    {
        if(delete_map_list[i]["field"] == "ImageID")
        {
            ImageID = delete_map_list[i]["value"];

            break;
        }
    }

    QJsonObject delete_bbox_Obj;

    delete_bbox_Obj.insert("ImageID", ImageID);

    for(int i = 0; i < network_type_list_.size(); ++i)
    {
        if(!deleteMaskResult(network_type_list_[i], delete_bbox_Obj))
        {
//            qDebug() << "DataManager::deleteSourceImage : no mask data.";
//            return false;
        }

        if(!deletePredictResult(network_type_list_[i], delete_bbox_Obj))
        {
            //            qDebug() << "DataManager::deleteSourceImage : no predict data.";
//            return false;
        }
    }

//    querySourceImage(delete_bbox_Obj);

//    if(query_result_list_.size() == 0)
//    {
//        qDebug() << "DataManager::deleteSourceImage : no image data.";
//        return false;
//    }

//    if(query_result_list_.size() > 0)
//    {
//        for(int i = 0; i < network_type_list_.size(); ++i)
//        {
//            QString current_tag = query_result_list_[0].value("is" + network_type_translator_[network_type_list_[i]] + "Masked").toString();

//            if(current_tag == "1")
//            {
//                if(!deleteMaskResult(network_type_list_[i], delete_bbox_Obj))
//                {
////                    qDebug() << "DataManager::deleteSourceImage : no mask data.";
////                    return false;
//                }
//            }

//            current_tag = query_result_list_[0].value("is" + network_type_translator_[network_type_list_[i]] + "Predicted").toString();

//            if(current_tag == "1")
//            {
//                if(!deletePredictResult(network_type_list_[i], delete_bbox_Obj))
//                {
////                    qDebug() << "DataManager::deleteSourceImage : no predict data.";
////                    return false;
//                }
//            }
//        }
//    }

    return database_manager_->deleteRecord("SourceImage", delete_map_list);
}

bool DataManager::insertMaskResult(NetworkType network_type,
                                   QString ImageID,
                                   QString ID,
                                   QString Time,
                                   QString label,
                                   QString x1,
                                   QString x2,
                                   QString y1,
                                   QString y2)
{
    QJsonObject insert_Obj = maskBoundingBoxJson(ImageID,
                                                 ID,
                                                 Time,
                                                 label,
                                                 x1,
                                                 x2,
                                                 y1,
                                                 y2);

    return insertMaskResult(network_type, insert_Obj);
}

bool DataManager::insertMaskResult(NetworkType network_type, QJsonObject insert_Obj)
{
    QMap<QString, QString> insert_map = jsonToMap(insert_Obj);

    if(!insert_map.keys().contains("ImageID"))
    {
        qDebug() << "DataManager::insertMaskResult : input doesn't have ImageID.";
        return false;
    }

    if(!insert_map.keys().contains("Time"))
    {
        QString current_Time = QDateTime::currentDateTime().toString("yy/MM/dd hh:mm:ss");

        insert_map.insert("Time", current_Time);
    }

    if(!insert_map.keys().contains("ID"))
    {
        QJsonObject query_Obj;
        query_Obj.insert("ImageID", insert_map["ImageID"]);

        queryMaskResult(network_type, query_Obj);

        QStringList ID_list;

        if(query_result_list_.size() > 0)
        {
            for(int i = 0; i < query_result_list_.size(); ++i)
            {
                QString query_ID = query_result_list_[i].value("ID").toString();

                ID_list.append(query_ID);
            }
        }

        int insert_ID = 0;

        while(ID_list.contains(QString::number(insert_ID)))
        {
            ++insert_ID;
        }

        insert_map.insert("ID", QString::number(insert_ID));
    }

    if(!database_manager_->insertRecord(network_type_translator_[network_type] + "Mask", insert_map))
    {
        return false;
    }

    QJsonObject revise_Obj;

    revise_Obj.insert("ImageID", insert_map["ImageID"]);
    revise_Obj.insert("is" + network_type_translator_[network_type] + "Masked", "1");

    return reviseSourceImage(revise_Obj);
}

bool DataManager::queryMaskResult(NetworkType network_type,
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
                                  QString y2)
{
    QJsonObject query_Obj = maskBoundingBoxWithOperatorJson(ImageIDOperator,
                                                            ImageID,
                                                            IDOperator,
                                                            ID,
                                                            TimeOperator,
                                                            Time,
                                                            labelOperator,
                                                            label,
                                                            x1Operator,
                                                            x1,
                                                            x2Operator,
                                                            x2,
                                                            y1Operator,
                                                            y1,
                                                            y2Operator,
                                                            y2);

    return queryMaskResult(network_type, query_Obj);
}

bool DataManager::queryMaskResult(NetworkType network_type, QJsonObject query_Obj)
{
    QList<QMap<QString, QString>> query_map_list = jsonToMapList(query_Obj);

    if(query_map_list.size() == 0)
    {
        qDebug() << "DataManager::queryMaskResult : empty input.";
        return false;
    }

    query_result_list_.clear();

    if(!database_manager_->queryRecord(network_type_translator_[network_type] + "Mask", query_map_list))
    {
        return false;
    }

    while(database_manager_->query_->next())
    {
        QString ImageID = database_manager_->query_->value("ImageID").toString();
        QString ID = database_manager_->query_->value("ID").toString();
        QString Time = database_manager_->query_->value("Time").toString();
        QString label = database_manager_->query_->value("label").toString();
        QString x1 = database_manager_->query_->value("x1").toString();
        QString y1 = database_manager_->query_->value("y1").toString();
        QString x2 = database_manager_->query_->value("x2").toString();
        QString y2 = database_manager_->query_->value("y2").toString();

        QJsonObject query_save_Obj = maskBoundingBoxJson(ImageID,
                                                         ID,
                                                         Time,
                                                         label,
                                                         x1,
                                                         x2,
                                                         y1,
                                                         y2);

        query_result_list_.append(query_save_Obj);
    }

    return true;
}

bool DataManager::deleteMaskResult(NetworkType network_type,
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
                                   QString y2)
{
    QJsonObject delete_Obj = maskBoundingBoxWithOperatorJson(ImageIDOperator,
                                                             ImageID,
                                                             IDOperator,
                                                             ID,
                                                             TimeOperator,
                                                             Time,
                                                             labelOperator,
                                                             label,
                                                             x1Operator,
                                                             x1,
                                                             x2Operator,
                                                             x2,
                                                             y1Operator,
                                                             y1,
                                                             y2Operator,
                                                             y2);

    return deleteMaskResult(network_type, delete_Obj);
}

bool DataManager::deleteMaskResult(NetworkType network_type, QJsonObject delete_Obj)
{
    QList<QMap<QString, QString>> delete_map_list = jsonToMapList(delete_Obj);

    if(delete_map_list.size() == 0)
    {
        qDebug() << "DataManager::deleteMaskResult : empty input.";
        return false;
    }

    return database_manager_->deleteRecord(network_type_translator_[network_type] + "Mask", delete_map_list);
}

bool DataManager::insertPredictResult(NetworkType network_type,
                                      QString ImageID,
                                      QString ID,
                                      QString Time,
                                      QString label,
                                      QString x1,
                                      QString x2,
                                      QString y1,
                                      QString y2,
                                      QString score)
{
    QJsonObject insert_Obj = predictBoundingBoxJson(ImageID,
                                                    ID,
                                                    Time,
                                                    label,
                                                    x1,
                                                    x2,
                                                    y1,
                                                    y2,
                                                    score);

    return insertPredictResult(network_type, insert_Obj);
}

bool DataManager::insertPredictResult(NetworkType network_type, QJsonObject insert_Obj)
{
    QMap<QString, QString> insert_map = jsonToMap(insert_Obj);

    if(!insert_map.keys().contains("ImageID"))
    {
        qDebug() << "DataManager::insertPredictResult : input doesn't have ImageID.";
        return false;
    }

    if(!insert_map.keys().contains("Time"))
    {
        QString current_Time = QDateTime::currentDateTime().toString("yy/MM/dd hh:mm:ss");

        insert_map.insert("Time", current_Time);
    }

    if(!database_manager_->insertRecord(network_type_translator_[network_type] + "Predict", insert_map))
    {
        return false;
    }

    QJsonObject revise_Obj;

    revise_Obj.insert("ImageID", insert_map["ImageID"]);
    revise_Obj.insert("is" + network_type_translator_[network_type] + "Predicted", "1");

    return reviseSourceImage(revise_Obj);
}

bool DataManager::queryPredictResult(NetworkType network_type,
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
                                     QString score)
{
    QJsonObject query_Obj = predictBoundingBoxWithOperatorJson(ImageIDOperator,
                                                               ImageID,
                                                               IDOperator,
                                                               ID,
                                                               TimeOperator,
                                                               Time,
                                                               labelOperator,
                                                               label,
                                                               x1Operator,
                                                               x1,
                                                               x2Operator,
                                                               x2,
                                                               y1Operator,
                                                               y1,
                                                               y2Operator,
                                                               y2,
                                                               scoreOperator,
                                                               score);

    return queryPredictResult(network_type, query_Obj);
}

bool DataManager::queryPredictResult(NetworkType network_type, QJsonObject query_Obj)
{
    QList<QMap<QString, QString>> query_map_list = jsonToMapList(query_Obj);

    if(query_map_list.size() == 0)
    {
        qDebug() << "DataManager::queryPredictResult : empty input.";
        return false;
    }

    query_result_list_.clear();

    if(!database_manager_->queryRecord(network_type_translator_[network_type] + "Predict", query_map_list))
    {
        return false;
    }

    while(database_manager_->query_->next())
    {
        QString ImageID = database_manager_->query_->value("ImageID").toString();
        QString ID = database_manager_->query_->value("ID").toString();
        QString Time = database_manager_->query_->value("Time").toString();
        QString label = database_manager_->query_->value("label").toString();
        QString x1 = database_manager_->query_->value("x1").toString();
        QString y1 = database_manager_->query_->value("y1").toString();
        QString x2 = database_manager_->query_->value("x2").toString();
        QString y2 = database_manager_->query_->value("y2").toString();
        QString score = database_manager_->query_->value("score").toString();

        QJsonObject query_save_Obj = predictBoundingBoxJson(ImageID,
                                                            ID,
                                                            Time,
                                                            label,
                                                            x1,
                                                            x2,
                                                            y1,
                                                            y2,
                                                            score);

        query_result_list_.append(query_save_Obj);
    }

    return true;
}

bool DataManager::deletePredictResult(NetworkType network_type,
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
                                      QString score)
{
    QJsonObject delete_Obj = predictBoundingBoxWithOperatorJson(ImageIDOperator,
                                                                ImageID,
                                                                IDOperator,
                                                                ID,
                                                                TimeOperator,
                                                                Time,
                                                                labelOperator,
                                                                label,
                                                                x1Operator,
                                                                x1,
                                                                x2Operator,
                                                                x2,
                                                                y1Operator,
                                                                y1,
                                                                y2Operator,
                                                                y2,
                                                                scoreOperator,
                                                                score);

    return deletePredictResult(network_type, delete_Obj);
}

bool DataManager::deletePredictResult(NetworkType network_type, QJsonObject delete_Obj)
{
    QList<QMap<QString, QString>> delete_map_list = jsonToMapList(delete_Obj);

    if(delete_map_list.size() == 0)
    {
        qDebug() << "DataManager::deletePredictResult : empty input.";
        return false;
    }

    return database_manager_->deleteRecord(network_type_translator_[network_type] + "Predict", delete_map_list);
}

QList<QJsonObject> DataManager::getQueryResultList()
{
    return query_result_list_;
}

QString DataManager::getFileHash(QString file_path)
{
    QString abs_file_path = file_path;

    if(!dir_.isAbsolutePath(abs_file_path))
    {
        abs_file_path = dir_.absoluteFilePath(abs_file_path);
    }

    QFile file(abs_file_path);

    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "DataManager::getFileHash : file open failed.";

        return "";
    }

    QCryptographicHash Hash(QCryptographicHash::Md5);

    QByteArray buf = file.readAll();
    Hash.addData(buf);

    image_.loadFromData(buf);
    image_width_string_ = QString::number(image_.width());
    image_height_string_ = QString::number(image_.height());

    file.close();

    QByteArray md5 = Hash.result().toBase64();
    return QString::fromUtf8(md5).left(32);
}

bool DataManager::insertFromMaskJsonObj(NetworkType network_type, QString ImageID, QJsonObject mask_json_Obj, double min_x, double min_y, double max_x, double max_y)
{
    if(mask_json_Obj.find("Area").value().toObject().empty())
    {
        qDebug() << "DataManager::insertFromMaskJsonObj : empty input json.";
        return false;
    }

    QJsonArray Area_labels_array = mask_json_Obj.value("Area").toObject().value("labels").toArray();

    if(Area_labels_array.size() == 0)
    {
        qDebug() << "DataManager::insertFromMaskJsonObj : no mask data.[maybe not wrong, cancel return].";
//        return false;
    }

    QJsonObject delete_Obj;
    delete_Obj.insert("ImageID", ImageID);

    querySourceImage(delete_Obj);

    if(query_result_list_.size() == 0)
    {
        QString ImagePath = mask_json_Obj.value("image_file_path").toString();

        if(!insertFromImageFile(Yolov3Region, ImageID, ImagePath))
        {
            qDebug() << "DataManager::insertFromMaskJsonObj : insert image failed.";
            return false;
        }
    }

    switch(network_type)
    {
    case Yolov3Region:

        queryMaskResult(current_network_type_, delete_Obj);

        if(query_result_list_.size() > 0)
        {
            deleteMaskResult(current_network_type_, delete_Obj);
        }

        break;
    case Yolov3Component:
        break;
    case ResNetClassification:
        break;
    }

    QJsonArray Area_polygons_array = mask_json_Obj.value("Area").toObject().value("polygons").toArray();
    QJsonArray Area_shape_array = mask_json_Obj.value("Area").toObject().value("shape").toArray();
    QJsonArray Area_track_Array_array = mask_json_Obj.value("Area").toObject().value("track").toObject().value("Array").toArray();

    QJsonObject revise_Obj;

    if(Area_labels_array.size() > 0)
    {
        for(int i = 0; i < Area_labels_array.size(); ++i)
        {
            QString ID = QString::number(Area_track_Array_array[i].toInt());

            QString label = Area_labels_array[i].toArray()[0].toString();

            int image_width = Area_shape_array[0].toInt();
            int image_height = Area_shape_array[1].toInt();

            int x_min = Area_polygons_array[i].toArray()[0].toArray()[0].toInt();
            int y_min = Area_polygons_array[i].toArray()[0].toArray()[1].toInt();
            int x_max = Area_polygons_array[i].toArray()[2].toArray()[0].toInt();
            int y_max = Area_polygons_array[i].toArray()[2].toArray()[1].toInt();

            QString x1 = QString::number(1.0 * x_min * (max_x - min_x) / image_width + min_x);
            QString x2 = QString::number(1.0 * x_max * (max_x - min_x) / image_width + min_x);
            QString y1 = QString::number(1.0 * y_min * (max_y - min_y) / image_height + min_y);
            QString y2 = QString::number(1.0 * y_max * (max_y - min_y) / image_height + min_y);

            insertMaskResult(network_type,
                             ImageID,
                             "",
                             "",
                             label,
                             x1,
                             x2,
                             y1,
                             y2);
        }
    }
    else
    {
        revise_Obj.insert("is" + network_type_translator_[network_type] + "Masked", "1");
    }

    revise_Obj.insert("ImageID", ImageID);
//    revise_Obj.insert(network_type_translator_[network_type] + "MaskID", "-1");

    reviseSourceImage(revise_Obj);

//    updateSourceImageModel();

    return true;
}

bool DataManager::insertFromMaskJsonFile(NetworkType network_type, QString ImageID, QString mask_json_file_path, double min_x, double min_y, double max_x, double max_y)
{
    QFile mask_json_file(mask_json_file_path);

    if(!mask_json_file.open(QFile::ReadOnly))
    {
        qDebug() << "DataManager::insertFromMaskJsonFile : open json file failed.";
        return false;
    }

    QByteArray bytes = mask_json_file.readAll();

    mask_json_file.close();

    QJsonDocument mask_json_doc = QJsonDocument::fromJson(bytes);

    QJsonObject mask_json_Obj = mask_json_doc.object();

    mask_json_Obj.insert("image_file_path", mask_json_file_path.replace("/json/", "/").replace(".json", ".jpg"));

    return insertFromMaskJsonObj(network_type, ImageID, mask_json_Obj, min_x, min_y, max_x, max_y);
}

bool DataManager::insertFromImageFile(NetworkType network_type, QString ImageID, QString ImagePath)
{
    if(!dir_.exists(ImagePath))
    {
        qDebug() << "DataManager::insertFromImageFile : " + ImagePath + " not exist.";
        return false;
    }

    if(ImageID == "")
    {
        qDebug() << "DataManager::insertFromImageFile : get ImageID failed.";
        qDebug() << ImagePath;
        return false;
    }

    QString ImageWidth = image_width_string_;
    QString ImageHeight = image_height_string_;
    QString ImportTime = QDateTime::currentDateTime().toString("yy/MM/dd hh:mm:ss");

    QJsonObject query_Obj;
    query_Obj.insert("ImageID", ImageID);

    querySourceImage(query_Obj);

    if(query_result_list_.size() == 0)
    {
        QJsonObject insert_Obj = sourceImageJson(ImageID,
                                                 ImagePath,
                                                 ImageWidth,
                                                 ImageHeight,
                                                 ImportTime,
                                                 "0",
                                                 "-1",
                                                 "0",
                                                 "-1",
                                                 "0",
                                                 "-1",
                                                 "0",
                                                 "0",
                                                 "0");

        if(!insertSourceImage(insert_Obj))
        {
            qDebug() << "DataManager::insertFromImageFile : insert source image failed.";
            qDebug() << ImagePath;
            return false;
        }
    }

    switch(network_type)
    {
    case Yolov3Region:
        break;
    case Yolov3Component:
        break;
    case ResNetClassification:

        QJsonObject mask_Obj = maskBoundingBoxJson(ImageID,
                                                   "",
                                                   "",
                                                   "0",
                                                   "0.0",
                                                   "1.0",
                                                   "0.0",
                                                   "1.0");

        if(!insertMaskResult(ResNetClassification, mask_Obj))
        {
            qDebug() << "DataManager::insertFromImageFile : insert mask data failed.";
            qDebug() << ImagePath;
            return false;
        }

        break;
    }

    return true;
}

bool DataManager::insertFromImageFile(NetworkType network_type, QString ImagePath)
{
    QString ImageID = getFileHash(ImagePath);

    return insertFromImageFile(network_type, ImageID, ImagePath);
}

bool DataManager::insertFromImageFileWithJsonDetect(NetworkType network_type, QString ImageID, QString image_file_path, double min_x, double min_y, double max_x, double max_y)
{
    QString image_file_basename = image_file_path.right(image_file_path.size() - image_file_path.lastIndexOf("/") - 1).split(".")[0];

    QString mask_json_file_path = image_file_path.split(image_file_basename)[0] + "/json/" + image_file_basename + ".json";

    if(dir_.exists(mask_json_file_path))
    {
        return insertFromMaskJsonFile(network_type, ImageID, mask_json_file_path, min_x, min_y, max_x, max_y);
    }

    return insertFromImageFile(network_type, ImageID, image_file_path);
}

bool DataManager::insertFromMaskJsonFileFolder(NetworkType network_type, QString mask_file_path, double min_x, double min_y, double max_x, double max_y)
{
    QStringList image_filter;
    image_filter << "*.jpg";

    dir_.setNameFilters(image_filter);
    dir_.setFilter(QDir::NoDotAndDotDot | QDir::AllEntries);

    dir_.setPath(mask_file_path);

    QStringList mask_image_file_list = dir_.entryList();

    int current_insert_num = 0;

    for(QString mask_image_file_name : mask_image_file_list)
    {
        QString mask_image_file_path = mask_file_path + mask_image_file_name;

        QString ImageID = getFileHash(mask_image_file_name);

        if(!insertFromImageFileWithJsonDetect(network_type, ImageID, mask_image_file_path, min_x, min_y, max_x, max_y))
        {
            qDebug() << "DataManager::insertFromMaskJsonFileFolder : insert mask json data failed.";
            qDebug() << mask_image_file_path;
            return false;
        }

        ++current_insert_num;

        emit signal_MsgToShow("insert : " + QString::number(current_insert_num) + "/" + QString::number(mask_image_file_list.size()));
    }

//    updateSourceImageModel();

    emit signal_MsgToShow("insert finished.");

    return true;
}

QJsonObject DataManager::toClassificationMaskJsonObj(QString ImageID, double min_x, double min_y, double max_x, double max_y)
{
    QJsonObject mask_json_Obj;

    QJsonObject query_Obj;
    query_Obj.insert("ImageID", ImageID);

    querySourceImage(query_Obj);

    if(query_result_list_.size() == 0)
    {
        qDebug() << "DataManager::toMaskJsonObj : query image info failed.";
        return mask_json_Obj;
    }

    int ImageWidth = query_result_list_[0].value("ImageWidth").toString().toInt();
    int ImageHeight = query_result_list_[0].value("ImageHeight").toString().toInt();

    ImageWidth = int(ImageWidth * (max_x - min_x));
    ImageHeight = int(ImageHeight * (max_y - min_y));

    QJsonObject Area_Obj;
    QJsonArray Area_labels_array;
    QJsonArray Area_polygons_array;
    QJsonArray Area_shape_Array;
    QJsonObject Area_track_Obj;
    QJsonArray Area_track_Array_array;

    QString ID = "0";
    QString Time = QDateTime::currentDateTime().toString("yy/MM/dd hh:mm:ss");
    QString label = "0";
    double x1 = 0.0;
    double x2 = 1.0;
    double y1 = 0.0;
    double y2 = 1.0;

    QJsonArray Area_labels_label_array;
    Area_labels_label_array.append(label);
    Area_labels_array.append(Area_labels_label_array);

    QJsonArray Area_polygons_polygon_array;
    QJsonArray Area_polygons_polygon_point1_array;
    Area_polygons_polygon_point1_array.append(int(x1 * ImageWidth));
    Area_polygons_polygon_point1_array.append(int(y1 * ImageHeight));
    Area_polygons_polygon_array.append(Area_polygons_polygon_point1_array);
    QJsonArray Area_polygons_polygon_point2_array;
    Area_polygons_polygon_point2_array.append(int(x1 * ImageWidth));
    Area_polygons_polygon_point2_array.append(int(y2 * ImageHeight));
    Area_polygons_polygon_array.append(Area_polygons_polygon_point2_array);
    QJsonArray Area_polygons_polygon_point3_array;
    Area_polygons_polygon_point3_array.append(int(x2 * ImageWidth));
    Area_polygons_polygon_point3_array.append(int(y2 * ImageHeight));
    Area_polygons_polygon_array.append(Area_polygons_polygon_point3_array);
    QJsonArray Area_polygons_polygon_point4_array;
    Area_polygons_polygon_point4_array.append(int(x2 * ImageWidth));
    Area_polygons_polygon_point4_array.append(int(y1 * ImageHeight));
    Area_polygons_polygon_array.append(Area_polygons_polygon_point4_array);
    Area_polygons_polygon_array.append(Area_polygons_polygon_point1_array);
    Area_polygons_array.append(Area_polygons_polygon_array);

    Area_track_Array_array.append(ID);

    Area_track_Obj["NextId"] = ID;

    Area_shape_Array.append(ImageWidth);
    Area_shape_Array.append(ImageHeight);

    Area_track_Obj.insert("Array", Area_track_Array_array);

    Area_Obj.insert("labels", Area_labels_array);
    Area_Obj.insert("polygons", Area_polygons_array);
    Area_Obj.insert("shape", Area_shape_Array);
    Area_Obj.insert("track", Area_track_Obj);
    Area_Obj.insert("type", "bbox");

    mask_json_Obj.insert("Area", Area_Obj);

    return mask_json_Obj;
}

QJsonObject DataManager::toMaskJsonObj(NetworkType network_type, QString ImageID, double min_x, double min_y, double max_x, double max_y, int targetImageWidth, int targetImageHeight)
{
    QJsonObject mask_json_Obj;

    QJsonObject query_Obj;
    query_Obj.insert("ImageID", ImageID);

    querySourceImage(query_Obj);

    if(query_result_list_.size() == 0)
    {
        qDebug() << "DataManager::toMaskJsonObj : query image info failed.";
        return mask_json_Obj;
    }

    int ImageWidth = query_result_list_[0].value("ImageWidth").toString().toInt();
    int ImageHeight = query_result_list_[0].value("ImageHeight").toString().toInt();

    if(targetImageWidth != -1)
    {
        ImageWidth = targetImageWidth;
    }
    if(targetImageHeight != -1)
    {
        ImageHeight = targetImageHeight;
    }

    ImageWidth = int(ImageWidth * (max_x - min_x));
    ImageHeight = int(ImageHeight * (max_y - min_y));

    queryMaskResult(network_type, query_Obj);

    QJsonObject Area_Obj;
    QJsonArray Area_labels_array;
    QJsonArray Area_polygons_array;
    QJsonArray Area_shape_Array;
    QJsonObject Area_track_Obj;
    QJsonArray Area_track_Array_array;

    Area_track_Obj["NextId"] = "0";

    if(query_result_list_.size() > 0)
    {
        for(int i = 0; i < query_result_list_.size(); ++i)
        {
            QString ID = query_result_list_[i].value("ID").toString();
            QString Time = query_result_list_[i].value("Time").toString();
            QString label = query_result_list_[i].value("label").toString();
            double x1 = query_result_list_[i].value("x1").toString().toDouble();
            double x2 = query_result_list_[i].value("x2").toString().toDouble();
            double y1 = query_result_list_[i].value("y1").toString().toDouble();
            double y2 = query_result_list_[i].value("y2").toString().toDouble();

            if(x1 < min_x || y1 < min_y || x2 > max_x || y2 > max_y)
            {
                continue;
            }

            x1 = (x1 - min_x) / (max_x - min_x);
            y1 = (y1 - min_y) / (max_y - min_y);
            x2 = (x2 - min_x) / (max_x - min_x);
            y2 = (y2 - min_y) / (max_y - min_y);

            QJsonArray Area_labels_label_array;
            Area_labels_label_array.append(label);
            Area_labels_array.append(Area_labels_label_array);

            QJsonArray Area_polygons_polygon_array;
            QJsonArray Area_polygons_polygon_point1_array;
            Area_polygons_polygon_point1_array.append(int(x1 * ImageWidth));
            Area_polygons_polygon_point1_array.append(int(y1 * ImageHeight));
            Area_polygons_polygon_array.append(Area_polygons_polygon_point1_array);
            QJsonArray Area_polygons_polygon_point2_array;
            Area_polygons_polygon_point2_array.append(int(x1 * ImageWidth));
            Area_polygons_polygon_point2_array.append(int(y2 * ImageHeight));
            Area_polygons_polygon_array.append(Area_polygons_polygon_point2_array);
            QJsonArray Area_polygons_polygon_point3_array;
            Area_polygons_polygon_point3_array.append(int(x2 * ImageWidth));
            Area_polygons_polygon_point3_array.append(int(y2 * ImageHeight));
            Area_polygons_polygon_array.append(Area_polygons_polygon_point3_array);
            QJsonArray Area_polygons_polygon_point4_array;
            Area_polygons_polygon_point4_array.append(int(x2 * ImageWidth));
            Area_polygons_polygon_point4_array.append(int(y1 * ImageHeight));
            Area_polygons_polygon_array.append(Area_polygons_polygon_point4_array);
            Area_polygons_polygon_array.append(Area_polygons_polygon_point1_array);
            Area_polygons_array.append(Area_polygons_polygon_array);

            Area_track_Array_array.append(ID);

            Area_track_Obj["NextId"] = ID;
        }
    }

    Area_shape_Array.append(ImageWidth);
    Area_shape_Array.append(ImageHeight);

    Area_track_Obj.insert("Array", Area_track_Array_array);

    Area_Obj.insert("labels", Area_labels_array);
    Area_Obj.insert("polygons", Area_polygons_array);
    Area_Obj.insert("shape", Area_shape_Array);
    Area_Obj.insert("track", Area_track_Obj);
    Area_Obj.insert("type", "bbox");

    mask_json_Obj.insert("Area", Area_Obj);

    return mask_json_Obj;
}

bool DataManager::toMaskJsonFile(NetworkType network_type, QString ImageID, QString mask_json_file_path, double min_x, double min_y, double max_x, double max_y, int targetImageWidth, int targetImageHeight)
{
    QJsonObject mask_json_Obj = toMaskJsonObj(network_type, ImageID, min_x, min_y, max_x, max_y, targetImageWidth, targetImageHeight);

    return toMaskJsonFile(mask_json_Obj, mask_json_file_path);
}

bool DataManager::toMaskJsonFile(QJsonObject mask_json_Obj, QString mask_json_file_path)
{
    QFile mask_json_file(mask_json_file_path);

    if(!mask_json_file.open(QFile::WriteOnly))
    {
        qDebug() << "DataManager::toMaskJsonFile : open json file failed.";
        return false;
    }

    mask_json_file.resize(0);

    QJsonDocument mask_json_doc = QJsonDocument(mask_json_Obj);

    QByteArray bytes = mask_json_doc.toJson();

    mask_json_file.write(bytes);

    mask_json_file.close();

    return true;
}

void DataManager::update_marked_and_unmarked_list()
{
    marked_file_id_.clear();
    marked_file_path_.clear();

    unmarked_file_id_.clear();
    unmarked_file_path_.clear();

    QJsonObject query_Obj;
    query_Obj.insert("ImageID", "null");
    query_Obj.insert("ImageIDOperator", "!=");

    QString mask_network_type = network_type_translator_[current_network_type_];

    query_Obj.insert("is" + mask_network_type + "Masked", "0");

    querySourceImage(query_Obj);

    for(int i = 0; i < query_result_list_.size(); ++i)
    {
        unmarked_file_id_.append(query_result_list_[i].value("ImageID").toString());
        unmarked_file_path_.append(query_result_list_[i].value("ImagePath").toString());
    }

    if(target_dataset_num_ >= unmarked_file_path_.size())
    {
        use_all_unmarked_dataset_ = true;
    }

    query_Obj["is" + mask_network_type + "Masked"] = "1";

    querySourceImage(query_Obj);

    for(int i = 0; i < query_result_list_.size(); ++i)
    {
        marked_file_id_.append(query_result_list_[i].value("ImageID").toString());
        marked_file_path_.append(query_result_list_[i].value("ImagePath").toString());
    }
}

void DataManager::loadFile()
{
    if(file_path_queue_.size() == 0)
    {
        return;
    }

    total_file_num_ = loaded_file_num_ + file_path_queue_.size();

    emit signal_MsgToShow("loaded images : " + QString::number(total_file_num_) + " / " + QString::number(find_files_num_) + " ...");

    for(int i = 0; i < total_file_num_ - loaded_file_num_; ++i)
    {
        QString ImagePath = file_path_queue_.dequeue();
        QString ImageID = getFileHash(ImagePath);
        QString ImageWidth = image_width_string_;
        QString ImageHeight = image_height_string_;
        QString ImportTime = QDateTime::currentDateTime().toString("yy/MM/dd hh:mm:ss");
        QString isYolov3RegionMasked = "0";
        QString Yolov3RegionMaskID = "-1";
        QString isYolov3ComponentMasked = "0";
        QString Yolov3ComponentMaskID = "-1";
        QString isResNetClassificationMasked = "0";
        QString ResNetClassificationMaskID = "-1";
        QString isYolov3RegionPredicted = "0";
        QString isYolov3ComponentPredicted = "0";
        QString isResNetClassificationPredicted = "0";

        if(ImageID == "")
        {
            qDebug() << "DataManagerWidget::loadFile : " + ImagePath + " open failed.";
            return;
        }

        QJsonObject insert_Obj = sourceImageJson(ImageID,
                                                 ImagePath,
                                                 ImageWidth,
                                                 ImageHeight,
                                                 ImportTime,
                                                 isYolov3RegionMasked,
                                                 Yolov3RegionMaskID,
                                                 isYolov3ComponentMasked,
                                                 Yolov3ComponentMaskID,
                                                 isResNetClassificationMasked,
                                                 ResNetClassificationMaskID,
                                                 isYolov3RegionPredicted,
                                                 isYolov3ComponentPredicted,
                                                 isResNetClassificationPredicted);

        insertSourceImage(insert_Obj);

        ++loaded_file_num_;

        insertToSourceImageModel(ImageID);

        emit signal_ChangeProgressNum(loaded_file_num_);
    }
}

void DataManager::create_work_station()
{
    emit signal_MsgToShow("start creating work station ...");

    dir_.setPath(target_path_);
    if(dir_.exists())
    {
        dir_.removeRecursively();
    }

    dir_.mkpath(target_path_);

    if(unmarked_file_path_.size() == 0 || workers_num_ < 0)
    {
        return;
    }

    for(int i = 0; i < workers_num_; ++i)
    {
        if(!dir_.exists(work_path_ + QString::number(i)))
        {
            dir_.mkpath(work_path_ + QString::number(i));
        }
    }

    if(use_all_unmarked_dataset_)
    {
        dataset_num_for_each_worker_ = int(unmarked_file_path_.size() / workers_num_);
        res_dataset_num_ = unmarked_file_path_.size() % workers_num_;
    }
    else
    {
        dataset_num_for_each_worker_ = int(target_dataset_num_ / workers_num_);
        res_dataset_num_ = target_dataset_num_ % workers_num_;
    }

    emit signal_MsgToShow("Work delivered finished. We have " + QString::number(workers_num_) + " workers, and each worker has " + QString::number(dataset_num_for_each_worker_) + " images to mark.");

    if(res_dataset_num_ > 0)
    {
        emit signal_MsgToShow("There are " + QString::number(res_dataset_num_) + " images more, which will be marked randomly by workers.");
    }

    copy_unmarked_dataset();
}

void DataManager::copy_unmarked_dataset()
{
    emit signal_MsgToShow("start copying unmarked dataset ...");

    if(unmarked_file_path_.size() == 0 || workers_num_ < 0)
    {
        return;
    }

    int skip_num = 1;

    int current_unmarked_dataset_index = 0;

    int total_work_num = workers_num_ * dataset_num_for_each_worker_ + res_dataset_num_;

    if(dataset_num_for_each_worker_ > 0)
    {
        skip_num = int(unmarked_file_path_.size() / total_work_num);

        for(int i = 0; i < workers_num_; ++i)
        {
            QString current_work_path = work_path_ + QString::number(i) + "/";

            for(int j = 0; j < dataset_num_for_each_worker_; ++j)
            {
                int current_mask_id = get_mask_id(current_network_type_);

                QString current_file_path = unmarked_file_path_[current_unmarked_dataset_index];

                QString target_file_path = current_work_path + QString::number(current_mask_id) + ".jpg";

                QJsonObject query_Obj;
                query_Obj.insert("ImageID", unmarked_file_id_[current_unmarked_dataset_index]);

                bool need_to_copy = false;

                switch(current_network_type_)
                {
                case Yolov3Region:

                    need_to_copy = true;

                    break;
                case Yolov3Component:

                    queryMaskResult(Yolov3Region, query_Obj);

                    if(query_result_list_.size() > 0)
                    {
                        need_to_copy = true;
                    }

                    break;
                case ResNetClassification:

                    queryMaskResult(Yolov3Component, query_Obj);

                    if(query_result_list_.size() > 0)
                    {
                        need_to_copy = true;
                    }

                    break;
                }

                if(need_to_copy)
                {
                    if(!copyImage(current_network_type_, unmarked_file_id_[current_unmarked_dataset_index], current_work_path, QString::number(current_mask_id)))
                    {
                        qDebug() << "DataManager::copy_unmarked_dataset : copy failed.";
                        qDebug() << "from : " << current_file_path;
                        qDebug() << "to   : " << target_file_path;
                        return;
                    }

                    QJsonObject revise_Obj;
                    revise_Obj.insert("ImageID", unmarked_file_id_[current_unmarked_dataset_index]);
                    revise_Obj.insert(network_type_translator_[current_network_type_] + "MaskID", QString::number(current_mask_id));

                    reviseSourceImage(revise_Obj);
                }
                else
                {
                    reuse_mask_id(current_network_type_, current_mask_id);
                }

                current_unmarked_dataset_index += skip_num;

                if((i * dataset_num_for_each_worker_ + j + 1) % debug_skip_num_ == 0)
                {
                    emit signal_MsgToShow("Copying files : " + QString::number(i * dataset_num_for_each_worker_ + j + 1) + " / " + QString::number(total_work_num) + " ...");
                }
            }
        }
    }

    if(res_dataset_num_ > 0)
    {
        current_unmarked_dataset_index -= skip_num - 1;

        for(int i = 0; i < res_dataset_num_; ++i)
        {
            int current_mask_id = get_mask_id(current_network_type_);

            QString current_work_path = work_path_ + QString::number(i) + "/";

            int j = dataset_num_for_each_worker_;

            QString current_file_path = unmarked_file_path_[current_unmarked_dataset_index];

            QString target_file_path = current_work_path + QString::number(current_mask_id) + ".jpg";

            QJsonObject query_Obj;
            query_Obj.insert("ImageID", unmarked_file_id_[current_unmarked_dataset_index]);

            bool need_to_copy = false;

            switch(current_network_type_)
            {
            case Yolov3Region:

                need_to_copy = true;

                break;
            case Yolov3Component:

                queryMaskResult(Yolov3Region, query_Obj);

                if(query_result_list_.size() > 0)
                {
                    need_to_copy = true;
                }

                break;
            case ResNetClassification:

                queryMaskResult(Yolov3Component, query_Obj);

                if(query_result_list_.size() > 0)
                {
                    need_to_copy = true;
                }

                break;
            }

            if(need_to_copy)
            {
                if(!copyImage(current_network_type_, unmarked_file_id_[current_unmarked_dataset_index], current_work_path, QString::number(current_mask_id)))
                {
                    qDebug() << "DataManager::copy_unmarked_dataset : copy failed.";
                    qDebug() << "from : " << current_file_path;
                    qDebug() << "to   : " << target_file_path;
                    return;
                }

                QJsonObject revise_Obj;
                revise_Obj.insert("ImageID", unmarked_file_id_[current_unmarked_dataset_index]);
                revise_Obj.insert(network_type_translator_[current_network_type_] + "MaskID", QString::number(current_mask_id));

                reviseSourceImage(revise_Obj);
            }
            else
            {
                reuse_mask_id(current_network_type_, current_mask_id);
            }

            ++current_unmarked_dataset_index;

            emit signal_MsgToShow("Copying files : " + QString::number((workers_num_ - 1) * dataset_num_for_each_worker_ + i + j + 1) + " / " + QString::number(total_work_num) + "...");
        }
    }

    updateSourceImageModel();

    emit signal_MsgToShow("Copy files finished.");
}

void DataManager::init_mask_id_list()
{    
    if(mask_id_queue_list_.size() > 0)
    {
        for(int i = 0; i < mask_id_queue_list_.size(); ++i)
        {
            mask_id_queue_list_[i].clear();
        }
    }

    mask_id_queue_list_.clear();

    new_mask_id_list_.clear();

    QJsonObject query_Obj;
    query_Obj.insert("ImageID", "null");
    query_Obj.insert("ImageIDOperator", "!=");

    querySourceImage(query_Obj);

    for(int i = 0; i < network_type_list_.size(); ++i)
    {
        QQueue<int> mask_id_queue;

        int new_mask_id = 0;

        QList<int> mask_id_list;

        if(query_result_list_.size() > 0)
        {
            for(int j = 0; j < query_result_list_.size(); ++j)
            {
                int current_mask_id = query_result_list_[j].value(network_type_translator_[network_type_list_[i]] + "MaskID").toString().toInt();

                if(current_mask_id != -1)
                {
                    mask_id_list.append(current_mask_id);

                    if(current_mask_id >= new_mask_id)
                    {
                        new_mask_id = current_mask_id + 1;
                    }
                }
            }

            if(new_mask_id > 0)
            {
                for(int j = 0; j < new_mask_id; ++j)
                {
                    if(!mask_id_list.contains(j))
                    {
                        mask_id_queue.enqueue(j);
                    }
                }
            }
        }

        mask_id_queue_list_.append(mask_id_queue);

        new_mask_id_list_.append(new_mask_id);
    }
}

int DataManager::get_mask_id(NetworkType network_type)
{
    int network_index = int(network_type);

    if(mask_id_queue_list_[network_index].size() > 0)
    {
        return mask_id_queue_list_[network_index].dequeue();
    }

    ++new_mask_id_list_[network_index];

    return new_mask_id_list_[network_index] - 1;
}

void DataManager::reuse_mask_id(NetworkType network_type, int mask_id)
{
    int network_index = int(network_type);

    if(!mask_id_queue_list_[network_index].contains(mask_id))
    {
        mask_id_queue_list_[network_index].enqueue(mask_id);

        if(mask_id >= new_mask_id_list_[network_index])
        {
            new_mask_id_list_[network_index] = mask_id + 1;
        }
    }
}

double DataManager::getIOU(double x1, double y1, double x2, double y2, double new_x1, double new_y1, double new_x2, double new_y2)
{
    double iou = 0;

    double x_min = fmax(x1, new_x1);
    double x_max = fmin(x2, new_x2);
    double y_min = fmax(y1, new_y1);
    double y_max = fmin(y2, new_y2);

    if(x_min >= x_max || y_min >= y_max)
    {
        return iou;
    }

    double common_area = (x_max - x_min) * (y_max - y_min);

    double total_area = (x2 - x1) * (y2 - y1) + (new_x2 - new_x1) * (new_y2 - new_y1) - common_area;

    iou = common_area / total_area;

    return iou;
}

bool DataManager::copyImage(NetworkType network_type, QString ImageID, QString mask_path, QString MaskID)
{
    QJsonObject query_Obj;
    query_Obj.insert("ImageID", ImageID);

    querySourceImage(query_Obj);

    if(query_result_list_.size() == 0)
    {
        qDebug() << "DataManager::copyImage : no image in db.";
        return false;
    }

    QString ImagePath = query_result_list_[0].value("ImagePath").toString();

    int ImageWidth = query_result_list_[0].value("ImageWidth").toString().toInt();
    int ImageHeight = query_result_list_[0].value("ImageHeight").toString().toInt();

    QImage source_image(ImagePath);
    QImage target_image;

    QString target_path = mask_path;

    bool success = true;
    bool list_have_success = false;

    QList<QJsonObject> mask_query_result_list;

    switch(network_type)
    {
    case Yolov3Region:
//        target_image = source_image.scaled(608, 608);
        target_image = source_image;

        if(!dir_.exists(target_path))
        {
            dir_.mkpath(target_path);
        }

        if(!target_image.save(target_path + MaskID + ".jpg", "JPG", 100))
        {
            qDebug() << "DataManager::copyImageWithJson : copy image failed.";
            success = false;
            break;
        }

        break;
    case Yolov3Component:
        queryMaskResult(Yolov3Region, query_Obj);

        if(query_result_list_.size() == 0)
        {
            qDebug() << "DataManager::copyImage : no mask data in Yolov3Region db.";
            success = false;
            break;
        }

        for(int i = 0; i < query_result_list_.size(); ++i)
        {
            double x1 = query_result_list_[i].value("x1").toString().toDouble();
            double y1 = query_result_list_[i].value("y1").toString().toDouble();
            double x2 = query_result_list_[i].value("x2").toString().toDouble();
            double y2 = query_result_list_[i].value("y2").toString().toDouble();

            int pixel_x1 = int(x1 * ImageWidth);
            int pixel_y1 = int(y1 * ImageHeight);
            int pixel_x2 = int(x2 * ImageWidth);
            int pixel_y2 = int(y2 * ImageHeight);

            QString ID = query_result_list_[i].value("ID").toString();

            target_image = source_image.copy(pixel_x1, pixel_y1, pixel_x2-pixel_x1, pixel_y2-pixel_y1);

            if(!dir_.exists(target_path))
            {
                dir_.mkpath(target_path);
            }

            if(!target_image.save(target_path + MaskID + "_" + ID + ".jpg", "JPG", 100))
            {
                success = false;
                continue;
            }

            list_have_success = true;
        }

        break;
    case ResNetClassification:
        queryMaskResult(Yolov3Component, query_Obj);

        if(query_result_list_.size() == 0)
        {
            qDebug() << "DataManager::copyImage : no mask data in Yolov3Region db.";
            success = false;
            break;
        }

        mask_query_result_list = query_result_list_;

        for(int i = 0; i < mask_query_result_list.size(); ++i)
        {
            double x1 = mask_query_result_list[i].value("x1").toString().toDouble();
            double y1 = mask_query_result_list[i].value("y1").toString().toDouble();
            double x2 = mask_query_result_list[i].value("x2").toString().toDouble();
            double y2 = mask_query_result_list[i].value("y2").toString().toDouble();

            QString label = mask_query_result_list[i].value("label").toString();

            int pixel_x1 = int(x1 * ImageWidth);
            int pixel_y1 = int(y1 * ImageHeight);
            int pixel_x2 = int(x2 * ImageWidth);
            int pixel_y2 = int(y2 * ImageHeight);

            QString ID = mask_query_result_list[i].value("ID").toString();

            target_path = mask_path + label + "/";

            target_image = source_image.copy(pixel_x1, pixel_y1, pixel_x2-pixel_x1, pixel_y2-pixel_y1);

            if(!dir_.exists(target_path))
            {
                dir_.mkpath(target_path);
            }

            if(!target_image.save(target_path + MaskID + "_" + ID + ".jpg", "JPG", 100))
            {
                success = false;
                continue;
            }

            QJsonObject classification_mask_Obj = toClassificationMaskJsonObj(ImageID, x1, y1, x2, y2);

            if(!dir_.exists(target_path + "json/"))
            {
                dir_.mkpath(target_path + "json/");
            }

            if(!toMaskJsonFile(classification_mask_Obj, target_path + "json/" + MaskID + "_" + ID + ".json"))
            {
                success = false;
                continue;
            }
            list_have_success = true;
        }

        break;
    }

    if(list_have_success)
    {
        return true;
    }

    return success;
}

bool DataManager::copyImageWithMaskJson(NetworkType network_type, QString ImageID, QString mask_path, QString MaskID)
{
    QJsonObject query_Obj;
    query_Obj.insert("ImageID", ImageID);

    querySourceImage(query_Obj);

    if(query_result_list_.size() == 0)
    {
        qDebug() << "DataManager::copyImage : no image in db.";
        return false;
    }

    QString ImagePath = query_result_list_[0].value("ImagePath").toString();

    int ImageWidth = query_result_list_[0].value("ImageWidth").toString().toInt();
    int ImageHeight = query_result_list_[0].value("ImageHeight").toString().toInt();

    QImage source_image(ImagePath);
    QImage target_image;

    QString target_path = mask_path;

    bool success = true;
    bool list_have_success = false;

    int targetImageWidth = -1;
    int targetImageHeight = -1;

    QList<QJsonObject> mask_query_result_list;

    QList<QJsonObject> label_query_result_list;

    switch(network_type)
    {
    case Yolov3Region:
//        targetImageWidth = 608;
//        targetImageHeight = 608;

//        target_image = source_image.scaled(targetImageWidth, targetImageHeight);
        target_image = source_image;

        if(!dir_.exists(target_path))
        {
            dir_.mkpath(target_path);
        }

        if(!target_image.save(target_path + MaskID + ".jpg", "JPG", 100))
        {
            qDebug() << "DataManager::copyImageWithJson : copy image failed.";
            success = false;
            break;
        }

        if(!dir_.exists(target_path + "json/"))
        {
            dir_.mkpath(target_path + "json/");
        }

        if(!toMaskJsonFile(network_type, ImageID, target_path + "json/" + MaskID + ".json", 0.0, 0.0, 1.0, 1.0, targetImageWidth, targetImageHeight))
        {
            success = false;
            break;
        }

        break;
    case Yolov3Component:
        if(connect_mask_)
        {
            queryMaskResult(Yolov3Region, query_Obj);

            if(query_result_list_.size() == 0)
            {
                qDebug() << "DataManager::copyImage : no mask data in Yolov3Region db.";
                success = false;
                break;
            }

            mask_query_result_list = query_result_list_;
        }
        else
        {
            mask_query_result_list.clear();

            QJsonObject mask_Obj = maskBoundingBoxJson(ImageID,
                                                       "0",
                                                       "",
                                                       "0",
                                                       "0.0",
                                                       "1.0",
                                                       "0.0",
                                                       "1.0");

            mask_query_result_list.append(mask_Obj);
        }

        for(int i = 0; i < mask_query_result_list.size(); ++i)
        {
            double x1 = mask_query_result_list[i].value("x1").toString().toDouble();
            double y1 = mask_query_result_list[i].value("y1").toString().toDouble();
            double x2 = mask_query_result_list[i].value("x2").toString().toDouble();
            double y2 = mask_query_result_list[i].value("y2").toString().toDouble();

            int pixel_x1 = int(x1 * ImageWidth);
            int pixel_y1 = int(y1 * ImageHeight);
            int pixel_x2 = int(x2 * ImageWidth);
            int pixel_y2 = int(y2 * ImageHeight);

            QString ID = mask_query_result_list[i].value("ID").toString();

            target_image = source_image.copy(pixel_x1, pixel_y1, pixel_x2-pixel_x1, pixel_y2-pixel_y1);

            if(!dir_.exists(target_path))
            {
                dir_.mkpath(target_path);
            }

            if(!target_image.save(target_path + MaskID + "_" + ID + ".jpg", "JPG", 100))
            {
                success = false;
                continue;
            }

            if(!dir_.exists(target_path + "json/"))
            {
                dir_.mkpath(target_path + "json/");
            }

            if(!toMaskJsonFile(network_type, ImageID, target_path + "json/" + MaskID + "_" + ID + ".json", x1, y1, x2, y2, targetImageWidth, targetImageHeight))
            {
                success = false;
                continue;
            }
            list_have_success = true;
        }

        break;
    case ResNetClassification:

        if(connect_mask_)
        {
            queryMaskResult(Yolov3Component, query_Obj);

            if(query_result_list_.size() == 0)
            {
                qDebug() << "DataManager::copyImage : no mask data in Yolov3Region db.";
                success = false;
                break;
            }

            mask_query_result_list = query_result_list_;
        }
        else
        {
            mask_query_result_list.clear();

            QJsonObject mask_Obj = maskBoundingBoxJson(ImageID,
                                                       "0",
                                                       "",
                                                       "0",
                                                       "0.0",
                                                       "1.0",
                                                       "0.0",
                                                       "1.0");

            mask_query_result_list.append(mask_Obj);
        }

        queryMaskResult(ResNetClassification, query_Obj);

        label_query_result_list = query_result_list_;

        for(int i = 0; i < mask_query_result_list.size(); ++i)
        {
            double x1 = mask_query_result_list[i].value("x1").toString().toDouble();
            double y1 = mask_query_result_list[i].value("y1").toString().toDouble();
            double x2 = mask_query_result_list[i].value("x2").toString().toDouble();
            double y2 = mask_query_result_list[i].value("y2").toString().toDouble();

            QString label = mask_query_result_list[i].value("label").toString();

            int pixel_x1 = int(x1 * ImageWidth);
            int pixel_y1 = int(y1 * ImageHeight);
            int pixel_x2 = int(x2 * ImageWidth);
            int pixel_y2 = int(y2 * ImageHeight);

            QString ID = mask_query_result_list[i].value("ID").toString();

            int current_new_mask_index = -1;

            if(connect_mask_)
            {
                double max_iou = 0;

                for(int j = 0; j < label_query_result_list.size(); ++j)
                {
                    double new_x1 = label_query_result_list[j].value("x1").toString().toDouble();
                    double new_y1 = label_query_result_list[j].value("y1").toString().toDouble();
                    double new_x2 = label_query_result_list[j].value("x2").toString().toDouble();
                    double new_y2 = label_query_result_list[j].value("y2").toString().toDouble();

                    double current_iou = getIOU(x1, y1, x2, y2, new_x1, new_y1, new_x2, new_y2);

                    if(current_iou > max_iou)
                    {
                        current_new_mask_index = j;

                        max_iou = current_iou;
                    }
                }
            }
            else
            {
                current_new_mask_index = i;
            }

            if(current_new_mask_index == -1)
            {
                qDebug() << "DataManager::copyImageWithMaskJson : find matched new label failed.";
                continue;
            }

            QString new_label = label_query_result_list[current_new_mask_index].value("label").toString();

            target_path = mask_path + label + "/" + new_label + "/";

            target_image = source_image.copy(pixel_x1, pixel_y1, pixel_x2-pixel_x1, pixel_y2-pixel_y1);

            if(!dir_.exists(target_path))
            {
                dir_.mkpath(target_path);
            }

            if(!target_image.save(target_path + MaskID + "_" + ID + ".jpg", "JPG", 100))
            {
                success = false;
                continue;
            }

            if(!dir_.exists(target_path + "json/"))
            {
                dir_.mkpath(target_path + "json/");
            }

            if(!toMaskJsonFile(network_type, ImageID, target_path + "json/" + MaskID + "_" + ID + ".json", x1, y1, x2, y2, targetImageWidth, targetImageHeight))
            {
                success = false;
                continue;
            }
            list_have_success = true;
        }

        break;
    }

    if(list_have_success)
    {
        return true;
    }

    return success;
}

bool DataManager::copyImageWithTrainJson(NetworkType network_type, QString ImageID, QString mask_path, QString MaskID)
{
    QJsonObject query_Obj;
    query_Obj.insert("ImageID", ImageID);

    querySourceImage(query_Obj);

    if(query_result_list_.size() == 0)
    {
        qDebug() << "DataManager::copyImage : no image in db.";
        return false;
    }

    QString ImagePath = query_result_list_[0].value("ImagePath").toString();

    int ImageWidth = query_result_list_[0].value("ImageWidth").toString().toInt();
    int ImageHeight = query_result_list_[0].value("ImageHeight").toString().toInt();

    QImage source_image(ImagePath);
    QImage target_image;

    QString target_path = mask_path;

    bool success = true;
    bool list_have_success = false;

    int targetImageWidth = -1;
    int targetImageHeight = -1;

    QList<QJsonObject> mask_query_result_list;

    QList<QJsonObject> label_query_result_list;

    switch(network_type)
    {
    case Yolov3Region:
//        targetImageWidth = 608;
//        targetImageHeight = 608;

//        target_image = source_image.scaled(targetImageWidth, targetImageHeight);
        target_image = source_image;

        if(!dir_.exists(target_path))
        {
            dir_.mkpath(target_path);
        }

        if(!target_image.save(target_path + MaskID + ".jpg", "JPG", 100))
        {
            qDebug() << "DataManager::copyImageWithJson : copy image failed.";
            success = false;
            break;
        }

        if(!toMaskJsonFile(network_type, ImageID, target_path + MaskID + ".json", 0.0, 0.0, 1.0, 1.0, targetImageWidth, targetImageHeight))
        {
            success = false;
            break;
        }

        break;
    case Yolov3Component:
        if(connect_mask_)
        {
            queryMaskResult(Yolov3Region, query_Obj);

            if(query_result_list_.size() == 0)
            {
                qDebug() << "DataManager::copyImage : no mask data in Yolov3Region db.";
                success = false;
                break;
            }

            mask_query_result_list = query_result_list_;
        }
        else
        {
            mask_query_result_list.clear();

            QJsonObject mask_Obj = maskBoundingBoxJson(ImageID,
                                                       "0",
                                                       "",
                                                       "0",
                                                       "0.0",
                                                       "1.0",
                                                       "0.0",
                                                       "1.0");

            mask_query_result_list.append(mask_Obj);
        }

        for(int i = 0; i < mask_query_result_list.size(); ++i)
        {
            double x1 = mask_query_result_list[i].value("x1").toString().toDouble();
            double y1 = mask_query_result_list[i].value("y1").toString().toDouble();
            double x2 = mask_query_result_list[i].value("x2").toString().toDouble();
            double y2 = mask_query_result_list[i].value("y2").toString().toDouble();

            int pixel_x1 = int(x1 * ImageWidth);
            int pixel_y1 = int(y1 * ImageHeight);
            int pixel_x2 = int(x2 * ImageWidth);
            int pixel_y2 = int(y2 * ImageHeight);

            QString ID = mask_query_result_list[i].value("ID").toString();

            target_image = source_image.copy(pixel_x1, pixel_y1, pixel_x2-pixel_x1, pixel_y2-pixel_y1);

            if(!dir_.exists(target_path))
            {
                dir_.mkpath(target_path);
            }

            if(!target_image.save(target_path + MaskID + "_" + ID + ".jpg", "JPG", 100))
            {
                success = false;
                continue;
            }

            if(!toMaskJsonFile(network_type, ImageID, target_path + MaskID + "_" + ID + ".json", x1, y1, x2, y2, targetImageWidth, targetImageHeight))
            {
                success = false;
                continue;
            }
            list_have_success = true;
        }

        break;
    case ResNetClassification:

        if(connect_mask_)
        {
            queryMaskResult(Yolov3Component, query_Obj);

            if(query_result_list_.size() == 0)
            {
                qDebug() << "DataManager::copyImage : no mask data in Yolov3Region db.";
                success = false;
                break;
            }

            mask_query_result_list = query_result_list_;
        }
        else
        {
            mask_query_result_list.clear();

            QJsonObject mask_Obj = maskBoundingBoxJson(ImageID,
                                                       "0",
                                                       "",
                                                       "0",
                                                       "0.0",
                                                       "1.0",
                                                       "0.0",
                                                       "1.0");

            mask_query_result_list.append(mask_Obj);
        }

        queryMaskResult(ResNetClassification, query_Obj);

        label_query_result_list = query_result_list_;

        for(int i = 0; i < mask_query_result_list.size(); ++i)
        {
            double x1 = mask_query_result_list[i].value("x1").toString().toDouble();
            double y1 = mask_query_result_list[i].value("y1").toString().toDouble();
            double x2 = mask_query_result_list[i].value("x2").toString().toDouble();
            double y2 = mask_query_result_list[i].value("y2").toString().toDouble();

            QString label = mask_query_result_list[i].value("label").toString();

            int pixel_x1 = int(x1 * ImageWidth);
            int pixel_y1 = int(y1 * ImageHeight);
            int pixel_x2 = int(x2 * ImageWidth);
            int pixel_y2 = int(y2 * ImageHeight);

            QString ID = mask_query_result_list[i].value("ID").toString();

            int current_new_mask_index = -1;

            if(connect_mask_)
            {
                double max_iou = 0;

                for(int j = 0; j < label_query_result_list.size(); ++j)
                {
                    double new_x1 = label_query_result_list[j].value("x1").toString().toDouble();
                    double new_y1 = label_query_result_list[j].value("y1").toString().toDouble();
                    double new_x2 = label_query_result_list[j].value("x2").toString().toDouble();
                    double new_y2 = label_query_result_list[j].value("y2").toString().toDouble();

                    double current_iou = getIOU(x1, y1, x2, y2, new_x1, new_y1, new_x2, new_y2);

                    if(current_iou > max_iou)
                    {
                        current_new_mask_index = j;

                        max_iou = current_iou;
                    }
                }
            }
            else
            {
                current_new_mask_index = i;
            }

            if(current_new_mask_index == -1)
            {
                qDebug() << "DataManager::copyImageWithMaskJson : find matched new label failed.";
                continue;
            }

            QString new_label = label_query_result_list[current_new_mask_index].value("label").toString();

            target_path = mask_path + label + "/" + new_label + "/";

            target_image = source_image.copy(pixel_x1, pixel_y1, pixel_x2-pixel_x1, pixel_y2-pixel_y1);

            if(!dir_.exists(target_path))
            {
                dir_.mkpath(target_path);
            }

            if(!target_image.save(target_path + MaskID + "_" + ID + ".jpg", "JPG", 100))
            {
                success = false;
                continue;
            }
            list_have_success = true;
        }

        break;
    }

    if(list_have_success)
    {
        return true;
    }

    return success;
}

void DataManager::init_param(QString root_path, QString target_path, int target_dataset_num, int workers_num)
{
    root_path_= root_path;
    target_path_ = target_path + network_type_translator_[current_network_type_] + "/";
    target_dataset_num_ = target_dataset_num;
    workers_num_ = workers_num;

    dataset_num_for_each_worker_ = 0;
    res_dataset_num_ = 0;
    work_path_ = target_path_ + "mask_station/";

    use_all_unmarked_dataset_ = false;

    update_marked_and_unmarked_list();

    init_mask_id_list();
}

void DataManager::create_mask_work_station()
{
    create_work_station();

    copy_unmarked_dataset();
}

void DataManager::recycle_mask_dataset()
{
    QStringList filter;
    filter << "*.json";
    dir_.setNameFilters(filter);
    dir_.setFilter(QDir::NoDotAndDotDot | QDir::AllEntries);

    int dataset_recycled_num = 0;
    int total_dataset_num = unmarked_file_path_.size();

    QDir folder_dir;
    folder_dir.setFilter(QDir::NoDotAndDotDot | QDir::AllEntries);

    QStringList folder_list;

    switch(current_network_type_)
    {
    case Yolov3Region:

        folder_dir.setPath(work_path_);
        folder_list = folder_dir.entryList();

        break;
    case Yolov3Component:

        folder_dir.setPath(work_path_);
        folder_list = folder_dir.entryList();

        break;
    case ResNetClassification:

        folder_dir.setPath(work_path_);

        QStringList label_folder_list = folder_dir.entryList();

        for(QString label_folder : label_folder_list)
        {
            folder_dir.setPath(work_path_ + label_folder + "/");

            QStringList label_work_folder_list = folder_dir.entryList();

            for(QString label_work_folder : label_work_folder_list)
            {
                folder_list.append(label_folder + "/" + label_work_folder);
            }
        }

        break;
    }

    int current_recycle_num = 0;

    for(int i = 0; i < folder_list.size(); ++i)
    {
        QString current_work_path = work_path_ + folder_list[i] + "/";

        QString current_json_path = current_work_path + "json/";

        if(!dir_.exists(current_json_path))
        {
            emit signal_MsgToShow("Folder " + QString::number(i) + " doesn't have any mark data.");
            continue;
        }

        dir_.setPath(current_json_path);

        QStringList current_json_file_list = dir_.entryList();

        dataset_recycled_num += current_json_file_list.size();

        for(QString json_file_name : current_json_file_list)
        {
            QString json_file_basename = json_file_name.split(".")[0];

            QString MaskID = "";
            QString ID = "";

            double min_x = 0.0;
            double min_y = 0.0;
            double max_x = 1.0;
            double max_y = 1.0;

            switch(current_network_type_)
            {
            case Yolov3Region:

                MaskID = json_file_basename;

                break;
            case Yolov3Component:

                MaskID = json_file_basename.split("_")[0];
                ID = json_file_basename.split("_")[1];

                break;
            case ResNetClassification:

                MaskID = json_file_basename.split("_")[0];
                ID = json_file_basename.split("_")[1];

                break;
            }

            QJsonObject query_Obj;
            query_Obj.insert(network_type_translator_[current_network_type_] + "MaskID", MaskID);

            querySourceImage(query_Obj);

            if(query_result_list_.size() != 1)
            {
                qDebug() << "DataManager::recycle_mask_dataset : cannot find source image with maskID = " << MaskID;
                return;
            }

            QString ImageID = query_result_list_[0].value("ImageID").toString();

            QJsonObject mask_query_Obj;
            mask_query_Obj.insert("ImageID", ImageID);

            if(connect_mask_)
            {
                switch(current_network_type_)
                {
                case Yolov3Region:

                    min_x = 0.0;
                    min_y = 0.0;
                    max_x = 1.0;
                    max_y = 1.0;

                    break;
                case Yolov3Component:

                    mask_query_Obj.insert("ID", ID);

                    queryMaskResult(Yolov3Region, mask_query_Obj);

                    if(query_result_list_.size() != 1)
                    {
                        qDebug() << "DataManager::recycle_mask_dataset : cannot find mask info with ID = " << ID;
                        return;
                    }

                    min_x = query_result_list_[0].value("x1").toString().toDouble();
                    min_y = query_result_list_[0].value("y1").toString().toDouble();
                    max_x = query_result_list_[0].value("x2").toString().toDouble();
                    max_y = query_result_list_[0].value("y2").toString().toDouble();

                    break;
                case ResNetClassification:

                    mask_query_Obj.insert("ID", ID);

                    queryMaskResult(Yolov3Component, mask_query_Obj);

                    if(query_result_list_.size() != 1)
                    {
                        qDebug() << "DataManager::recycle_mask_dataset : cannot find mask info with ID = " << ID;
                        return;
                    }

                    min_x = query_result_list_[0].value("x1").toString().toDouble();
                    min_y = query_result_list_[0].value("y1").toString().toDouble();
                    max_x = query_result_list_[0].value("x2").toString().toDouble();
                    max_y = query_result_list_[0].value("y2").toString().toDouble();

                    break;
                }
            }

            insertFromMaskJsonFile(current_network_type_, ImageID, current_json_path + json_file_name, min_x, min_y, max_x, max_y);

            reuse_mask_id(current_network_type_, MaskID.toInt());

            dir_.remove(current_work_path + json_file_basename + ".jpg");
            dir_.remove(current_json_path + json_file_name);

            ++current_recycle_num;

            emit signal_MsgToShow("recycling masked dataset : " + QString::number(current_recycle_num) + "...");
        }

        folder_dir.setFilter(QDir::NoDotAndDotDot | QDir::Files);

        folder_dir.setPath(current_work_path);

        QStringList image_list = folder_dir.entryList();

        if(image_list.size() == 0)
        {
            folder_dir.removeRecursively();
        }

        emit signal_MsgToShow("Recycling : recycled/total : " + QString::number(dataset_recycled_num) + " / " + QString::number(total_dataset_num));
    }

    updateSourceImageModel();

    update_marked_and_unmarked_list();

    emit signal_MsgToShow("Recycling finished : recycled/total : " + QString::number(dataset_recycled_num) + " / " + QString::number(total_dataset_num));
}

void DataManager::create_train_dataset()
{
    emit signal_MsgToShow("Starting creating train dataset, please wait.");

    QString train_dataset_path = target_path_ + "train_dataset/";

    dir_.setPath(train_dataset_path);

    if(dir_.exists())
    {
        dir_.removeRecursively();
    }

    dir_.mkpath(train_dataset_path);

    int current_img_idx = 0;

    for(int i = 0; i < marked_file_path_.size(); ++i)
    {
        if(!copyImageWithTrainJson(current_network_type_, marked_file_id_[i], train_dataset_path, QString::number(current_img_idx)))
        {
            qDebug() << "DataManager::create_train_dataset : copy img failed.";
            qDebug() << "from : " << marked_file_path_[i];
            qDebug() << "to   : " << train_dataset_path + QString::number(current_img_idx) + ".jpg";
//            return;
        }

        ++current_img_idx;

        if(current_img_idx % debug_skip_num_ == 0)
        {
            emit signal_MsgToShow("Creating train dataset : " + QString::number(current_img_idx) + " / " + QString::number(marked_file_path_.size()) + " ...");
        }
    }

    emit signal_MsgToShow("Creating train dataset finished. Dataset size :" + QString::number(current_img_idx));
}

void DataManager::create_valid_dataset()
{
    emit signal_MsgToShow("Starting creating valid dataset, please wait.");

    QString valid_dataset_path = target_path_ + "valid_dataset/";

    dir_.setPath(valid_dataset_path);

    if(dir_.exists())
    {
        dir_.removeRecursively();
    }

    dir_.mkpath(valid_dataset_path);

    int current_valid_image_num = 0;
    int current_process_num = 0;

    QJsonObject query_Obj;

    for(int i = 0; i < marked_file_path_.size(); ++i)
    {
        ++current_process_num;

        int current_img_idx = get_mask_id(current_network_type_);

        if(!copyImageWithMaskJson(current_network_type_, marked_file_id_[i], valid_dataset_path, QString::number(current_img_idx)))
        {
            qDebug() << "DataManager::create_valid_dataset : copy img failed.";
            qDebug() << "from : " << marked_file_path_[i];
            qDebug() << "to   : " << valid_dataset_path + QString::number(current_img_idx) + ".jpg";
            return;
        }

        QJsonObject revise_Obj;
        revise_Obj.insert("ImageID", marked_file_id_[i]);
        revise_Obj.insert(network_type_translator_[current_network_type_] + "MaskID", QString::number(current_img_idx));

        reviseSourceImage(revise_Obj);

        ++current_valid_image_num;

        if(current_valid_image_num % debug_skip_num_ == 0)
        {
            emit signal_MsgToShow("Creating valid dataset : " + QString::number(current_valid_image_num) + " / " + QString::number(marked_file_path_.size()) + " ...");
        }
    }

    updateSourceImageModel();

    emit signal_MsgToShow("Creating valid dataset finished. Dataset size :" + QString::number(current_valid_image_num));
}

void DataManager::recycle_valid_dataset()
{
    QStringList filter;
    filter << "*.json";
    dir_.setNameFilters(filter);
    dir_.setFilter(QDir::NoDotAndDotDot | QDir::AllEntries);

    QString current_work_path = target_path_ + "valid_dataset/";

    QStringList current_work_path_list;

    QDir folder_dir;
    folder_dir.setFilter(QDir::NoDotAndDotDot | QDir::AllEntries);

    switch(current_network_type_)
    {
    case Yolov3Region:

        current_work_path_list.append(current_work_path);

        break;
    case Yolov3Component:

        current_work_path_list.append(current_work_path);

        break;
    case ResNetClassification:

        folder_dir.setPath(current_work_path);

        QStringList current_sub_work_path_list = folder_dir.entryList();

        for(QString current_sub_folder : current_sub_work_path_list)
        {
            folder_dir.setPath(current_work_path + current_sub_folder + "/");

            QStringList current_sub_sub_work_folder_list = folder_dir.entryList();

            for(QString current_sub_sub_work_folder : current_sub_sub_work_folder_list)
            {
                current_work_path_list.append(current_work_path + current_sub_folder + "/" + current_sub_sub_work_folder + "/");
            }
        }

        break;
    }

    int current_recycle_num = 0;

    for(QString current_work_path : current_work_path_list)
    {
        QString current_json_path = current_work_path + "json/";

        if(!dir_.exists(current_json_path))
        {
            emit signal_MsgToShow("Valid dataset doesn't mask any image.");
            return;
        }

        dir_.setPath(current_json_path);

        QStringList current_json_file_list = dir_.entryList();

        if(!dir_.exists(current_json_path))
        {
            emit signal_MsgToShow("Folder doesn't have any mark data.");
            continue;
        }

        dir_.setPath(current_json_path);

        for(QString json_file_name : current_json_file_list)
        {
            QString json_file_basename = json_file_name.split(".")[0];

            QString MaskID = "";
            QString ID = "";

            double min_x = 0.0;
            double min_y = 0.0;
            double max_x = 1.0;
            double max_y = 1.0;

            switch(current_network_type_)
            {
            case Yolov3Region:

                MaskID = json_file_basename;

                break;
            case Yolov3Component:

                MaskID = json_file_basename.split("_")[0];
                ID = json_file_basename.split("_")[1];

                break;
            case ResNetClassification:

                MaskID = json_file_basename.split("_")[0];
                ID = json_file_basename.split("_")[1];

                break;
            }

            QJsonObject query_Obj;
            query_Obj.insert(network_type_translator_[current_network_type_] + "MaskID", MaskID);

            querySourceImage(query_Obj);

            if(query_result_list_.size() != 1)
            {
                qDebug() << "DataManager::recycle_valid_dataset : cannot find source image with maskID = " << MaskID;
                return;
            }

            QString ImageID = query_result_list_[0].value("ImageID").toString();

            QJsonObject mask_query_Obj;
            mask_query_Obj.insert("ImageID", ImageID);

            switch(current_network_type_)
            {
            case Yolov3Region:

                min_x = 0.0;
                min_y = 0.0;
                max_x = 1.0;
                max_y = 1.0;

                break;
            case Yolov3Component:

                mask_query_Obj.insert("ID", ID);

                if(connect_mask_)
                {
                    queryMaskResult(Yolov3Region, mask_query_Obj);

                    if(query_result_list_.size() != 1)
                    {
                        qDebug() << "DataManager::recycle_valid_dataset : cannot find mask info with ID = " << ID;
                        return;
                    }

                    min_x = query_result_list_[0].value("x1").toString().toDouble();
                    min_y = query_result_list_[0].value("y1").toString().toDouble();
                    max_x = query_result_list_[0].value("x2").toString().toDouble();
                    max_y = query_result_list_[0].value("y2").toString().toDouble();
                }

                break;
            case ResNetClassification:

                mask_query_Obj.insert("ID", ID);

                if(connect_mask_)
                {
                    queryMaskResult(Yolov3Component, mask_query_Obj);

                    if(query_result_list_.size() != 1)
                    {
                        qDebug() << "DataManager::recycle_mask_dataset : cannot find mask info with ID = " << ID;
                        return;
                    }

                    min_x = query_result_list_[0].value("x1").toString().toDouble();
                    min_y = query_result_list_[0].value("y1").toString().toDouble();
                    max_x = query_result_list_[0].value("x2").toString().toDouble();
                    max_y = query_result_list_[0].value("y2").toString().toDouble();
                }

                break;
            }

            queryMaskResult(current_network_type_, mask_query_Obj);

            if(query_result_list_.size() > 0)
            {
                deleteMaskResult(current_network_type_, mask_query_Obj);
            }

            insertFromMaskJsonFile(current_network_type_, ImageID, current_json_path + json_file_name, min_x, min_y, max_x, max_y);

            reuse_mask_id(current_network_type_, MaskID.toInt());

            dir_.remove(current_work_path + json_file_basename + ".jpg");
            dir_.remove(current_json_path + json_file_name);

            ++current_recycle_num;

            emit signal_MsgToShow("recycling masked dataset : " + QString::number(current_recycle_num) + "...");
        }
    }

    updateSourceImageModel();

    update_marked_and_unmarked_list();

    emit signal_MsgToShow("Recycling finished : " + QString::number(current_recycle_num) + " masked data recycled.");
}

void DataManager::slot_findFilesNum(int find_files_num)
{
    find_files_num_ = find_files_num;

    emit signal_ChangeProgressMaxNum(find_files_num_);

    emit signal_MsgToShow("scan folder finished : start load " + QString::number(find_files_num_) + " images.");

    database_manager_->database()->transaction();
}

void DataManager::slot_AddOpenFilePath(QString file_path)
{
    file_path_queue_.enqueue(file_path);

    loadFile();
}

void DataManager::slot_LoadDir_finished()
{
    database_manager_->database()->commit();

    updateSourceImageModel();
}

void DataManager::slot_CreateWorkStation()
{
    create_work_station();
}

void DataManager::slot_RecycleMask()
{
    recycle_mask_dataset();
}

void DataManager::slot_CreateTrainDataset()
{
    create_train_dataset();
}

void DataManager::slot_CreateValidDataset()
{
    create_valid_dataset();
}

void DataManager::slot_RecycleValid()
{
    recycle_valid_dataset();
}

void DataManager::slot_InsertFolder(QString insert_folder_path)
{
    insertFromMaskJsonFileFolder(current_network_type_, insert_folder_path, 0.0, 0.0, 1.0, 1.0);

    updateSourceImageModel();
}
