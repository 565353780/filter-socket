#include "DataManagerWidget.h"
#include "ui_datamanagerwidget.h"

DataManagerWidget::DataManagerWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DataManagerWidget)
{
    ui->setupUi(this);

    data_manager_thread_ = new QThread();
    data_manager_ = new DataManager();
    data_manager_moved_ = false;

    connect(data_manager_, SIGNAL(signal_MsgToShow(QString)), this, SLOT(slot_ShowMsg(QString)));

    connect(data_manager_, SIGNAL(signal_ChangeProgressMaxNum(int)), this, SLOT(slot_ChangeProgressMaxNum(int)));
    connect(data_manager_, SIGNAL(signal_ChangeProgressNum(int)), this, SLOT(slot_ChangeProgressNum(int)));

    connect(this, SIGNAL(signal_CreateWorkStation()), data_manager_, SLOT(slot_CreateWorkStation()));
    connect(this, SIGNAL(signal_RecycleMask()), data_manager_, SLOT(slot_RecycleMask()));
    connect(this, SIGNAL(signal_CreateTrainDataset()), data_manager_, SLOT(slot_CreateTrainDataset()));
    connect(this, SIGNAL(signal_CreateValidDataset()), data_manager_, SLOT(slot_CreateValidDataset()));
    connect(this, SIGNAL(signal_RecycleValid()), data_manager_, SLOT(slot_RecycleValid()));

    connect(this, SIGNAL(signal_InsertFolder(QString)), data_manager_, SLOT(slot_InsertFolder(QString)));

    data_loader_thread_ = new QThread();
    data_loader_ = new DataLoader();
    data_loader_->moveToThread(data_loader_thread_);
    data_loader_thread_->start();

    connect(this, SIGNAL(signal_setLoadFileNumForEachFolder(int)), data_loader_, SLOT(slot_setLoadFileNumForEachFolder(int)));

    connect(data_loader_, SIGNAL(signal_findFilesNum(int)), data_manager_, SLOT(slot_findFilesNum(int)));

    connect(this, SIGNAL(signal_LoadDir(QString, QStringList)), data_loader_, SLOT(slot_LoadDir(QString, QStringList)));

    connect(data_loader_, SIGNAL(signal_AddOpenFilePath(QString)), data_manager_, SLOT(slot_AddOpenFilePath(QString)));
    connect(data_loader_, SIGNAL(signal_LoadDir_finished()), data_manager_, SLOT(slot_LoadDir_finished()));

    current_open_dir_path_ = "/";

    ui->lineEdit_LoadFileNumForEachFolder->setText("-1");
}

DataManagerWidget::~DataManagerWidget()
{
    delete ui;
    delete data_loader_;
    delete data_manager_;
    delete data_loader_thread_;
    delete data_manager_thread_;
}

void DataManagerWidget::init(QString db_path)
{
    if(data_manager_moved_)
    {
        data_manager_->database_manager_->disconnect();
    }

    data_manager_->init(db_path);

    if(!data_manager_moved_)
    {
        data_manager_->moveToThread(data_manager_thread_);
        data_manager_thread_->start();
        data_manager_moved_ = true;
    }

    initSourceImageModel();

    initMaskModel();

    initPredictModel();

    data_manager_->current_network_type_ = data_manager_->network_type_list_[ui->cBox_MaskMode->currentIndex()];
}

void DataManagerWidget::initSourceImageModel()
{
    ui->tableView_SourceImage->setAlternatingRowColors(true);//使表格颜色交错功能为真
    ui->tableView_SourceImage->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);//表格宽度
    ui->tableView_SourceImage->setModel(data_manager_->model_sourceimage_);
//    ui->tableView_SourceImage->hideColumn(0); // don't show the ID
//    ui->tableView_SourceImage->hideColumn(4);
    ui->tableView_SourceImage->show();
}

void DataManagerWidget::initMaskModel()
{
    ui->tableView_Yolov3RegionMask->setAlternatingRowColors(true);//使表格颜色交错功能为真
    ui->tableView_Yolov3RegionMask->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);//表格宽度
    ui->tableView_Yolov3RegionMask->setModel(data_manager_->model_mask_Yolov3Region_);
//    ui->tableView_Yolov3RegionMask->hideColumn(0); // don't show the ID
//    ui->tableView_Yolov3RegionMask->hideColumn(2);
    ui->tableView_Yolov3RegionMask->show();

    ui->tableView_Yolov3ComponentMask->setAlternatingRowColors(true);//使表格颜色交错功能为真
    ui->tableView_Yolov3ComponentMask->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);//表格宽度
    ui->tableView_Yolov3ComponentMask->setModel(data_manager_->model_mask_Yolov3Component_);
//    ui->tableView_Yolov3ComponentMask->hideColumn(0); // don't show the ID
//    ui->tableView_Yolov3ComponentMask->hideColumn(2);
    ui->tableView_Yolov3ComponentMask->show();

    ui->tableView_ResNetClassificationMask->setAlternatingRowColors(true);//使表格颜色交错功能为真
    ui->tableView_ResNetClassificationMask->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);//表格宽度
    ui->tableView_ResNetClassificationMask->setModel(data_manager_->model_mask_ResNetClassification_);
//    ui->tableView_ResNetClassificationMask->hideColumn(0); // don't show the ID
//    ui->tableView_ResNetClassificationMask->hideColumn(2);
    ui->tableView_ResNetClassificationMask->show();
}

void DataManagerWidget::initPredictModel()
{
    ui->tableView_Yolov3RegionPredict->setAlternatingRowColors(true);//使表格颜色交错功能为真
    ui->tableView_Yolov3RegionPredict->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);//表格宽度
    ui->tableView_Yolov3RegionPredict->setModel(data_manager_->model_predict_Yolov3Region_);
//    ui->tableView_Yolov3RegionPredict->hideColumn(0); // don't show the ID
//    ui->tableView_Yolov3RegionPredict->hideColumn(2);
    ui->tableView_Yolov3RegionPredict->show();

    ui->tableView_Yolov3ComponentPredict->setAlternatingRowColors(true);//使表格颜色交错功能为真
    ui->tableView_Yolov3ComponentPredict->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);//表格宽度
    ui->tableView_Yolov3ComponentPredict->setModel(data_manager_->model_predict_Yolov3Component_);
//    ui->tableView_Yolov3ComponentPredict->hideColumn(0); // don't show the ID
//    ui->tableView_Yolov3ComponentPredict->hideColumn(2);
    ui->tableView_Yolov3ComponentPredict->show();

    ui->tableView_ResNetClassificationPredict->setAlternatingRowColors(true);//使表格颜色交错功能为真
    ui->tableView_ResNetClassificationPredict->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);//表格宽度
    ui->tableView_ResNetClassificationPredict->setModel(data_manager_->model_predict_ResNetClassification_);
//    ui->tableView_ResNetClassificationPredict->hideColumn(0); // don't show the ID
//    ui->tableView_ResNetClassificationPredict->hideColumn(2);
    ui->tableView_ResNetClassificationPredict->show();
}

bool DataManagerWidget::insertFromMaskJsonObj(DataManager::NetworkType network_type, QString ImageID, QJsonObject mask_json_Obj)
{
    if(mask_json_Obj.find("Area").value().toString() == "")
    {
        qDebug() << "DataManagerWidget::insertFromMaskJsonObj : empty input json.";
        return false;
    }

    QJsonArray Area_labels_array = mask_json_Obj.value("Area").toObject().value("labels").toArray();

    if(Area_labels_array.size() == 0)
    {
//        qDebug() << "DataManagerWidget::insertFromMaskJsonObj : no mask data.";
        return false;
    }

    QJsonArray Area_polygons_array = mask_json_Obj.value("Area").toObject().value("polygons").toArray();
    QJsonArray Area_shape_array = mask_json_Obj.value("Area").toObject().value("shape").toArray();
    QJsonArray Area_track_Array_array = mask_json_Obj.value("Area").toObject().value("track").toObject().value("Array").toArray();

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

        QString x1 = QString::number(1.0 * x_min / image_width);
        QString x2 = QString::number(1.0 * x_max / image_width);
        QString y1 = QString::number(1.0 * y_min / image_height);
        QString y2 = QString::number(1.0 * y_max / image_height);

        data_manager_->insertMaskResult(network_type,
                                        ImageID,
                                        ID,
                                        "",
                                        label,
                                        x1,
                                        x2,
                                        y1,
                                        y2);
    }

    return true;
}

QJsonObject DataManagerWidget::toMaskJsonObj(DataManager::NetworkType network_type, QString ImageID)
{
    QJsonObject mask_json_Obj;

    QJsonObject query_Obj;
    query_Obj.insert("ImageID", ImageID);

    data_manager_->querySourceImage(query_Obj);

    if(!data_manager_->database_manager_->query_->next())
    {
        qDebug() << "DataManagerWidget::toMaskJsonObj : query image info failed.";
        return mask_json_Obj;
    }

    int ImageWidth = data_manager_->database_manager_->query_->value("ImageWidth").toString().toInt();
    int ImageHeight = data_manager_->database_manager_->query_->value("ImageHeight").toString().toInt();

    data_manager_->queryMaskResult(network_type, query_Obj);

    QJsonObject Area_Obj;
    QJsonArray Area_labels_array;
    QJsonArray Area_polygons_array;
    QJsonArray Area_shape_Array;
    QJsonObject Area_track_Obj;
    QJsonArray Area_track_Array_array;

    while(data_manager_->database_manager_->query_->next())
    {
        QString ID = data_manager_->database_manager_->query_->value("ID").toString();
        QString Time = data_manager_->database_manager_->query_->value("Time").toString();
        QString label = data_manager_->database_manager_->query_->value("label").toString();
        QString x1 = data_manager_->database_manager_->query_->value("x1").toString();
        QString x2 = data_manager_->database_manager_->query_->value("x2").toString();
        QString y1 = data_manager_->database_manager_->query_->value("y1").toString();
        QString y2 = data_manager_->database_manager_->query_->value("y2").toString();

        QJsonArray Area_labels_label_array;
        Area_labels_label_array.append(label);
        Area_labels_array.append(Area_labels_label_array);

        QJsonArray Area_polygons_polygon_array;
        QJsonArray Area_polygons_polygon_point1_array;
        Area_polygons_polygon_point1_array.append(x1.toDouble() * ImageWidth);
        Area_polygons_polygon_point1_array.append(y1.toDouble() * ImageHeight);
        Area_polygons_polygon_array.append(Area_polygons_polygon_point1_array);
        QJsonArray Area_polygons_polygon_point2_array;
        Area_polygons_polygon_point2_array.append(x1.toDouble() * ImageWidth);
        Area_polygons_polygon_point2_array.append(y2.toDouble() * ImageHeight);
        Area_polygons_polygon_array.append(Area_polygons_polygon_point2_array);
        QJsonArray Area_polygons_polygon_point3_array;
        Area_polygons_polygon_point3_array.append(x2.toDouble() * ImageWidth);
        Area_polygons_polygon_point3_array.append(y2.toDouble() * ImageHeight);
        Area_polygons_polygon_array.append(Area_polygons_polygon_point3_array);
        QJsonArray Area_polygons_polygon_point4_array;
        Area_polygons_polygon_point4_array.append(x2.toDouble() * ImageWidth);
        Area_polygons_polygon_point4_array.append(y1.toDouble() * ImageHeight);
        Area_polygons_polygon_array.append(Area_polygons_polygon_point4_array);
        Area_polygons_polygon_array.append(Area_polygons_polygon_point1_array);
        Area_polygons_array.append(Area_polygons_polygon_array);

        Area_track_Array_array.append(ID);

        Area_track_Obj["NextId"] = ID;
    }

    Area_shape_Array.append(ImageWidth);
    Area_shape_Array.append(ImageHeight);

    Area_track_Obj.insert("Array", Area_track_Array_array);

    Area_Obj.insert("labels", Area_labels_array);
    Area_Obj.insert("polygons", Area_polygons_array);
    Area_Obj.insert("shape", Area_shape_Array);
    Area_Obj.insert("track", Area_track_Obj);
    Area_Obj.insert("type", "bbox");

    return mask_json_Obj;
}

bool DataManagerWidget::insertFromMaskJsonFile(DataManager::NetworkType network_type, QString ImageID, QString mask_json_file_path)
{
    QFile mask_json_file(mask_json_file_path);

    if(!mask_json_file.open(QFile::ReadOnly))
    {
        qDebug() << "DataManagerWidget::insertFromMaskJsonFile : open json file failed.";
        return false;
    }

    QByteArray bytes = mask_json_file.readAll();

    mask_json_file.close();

    QJsonDocument mask_json_doc = QJsonDocument::fromJson(bytes);

    QJsonObject mask_json_Obj = mask_json_doc.object();

    return insertFromMaskJsonObj(network_type, ImageID, mask_json_Obj);
}

bool DataManagerWidget::toMaskJsonFile(DataManager::NetworkType network_type, QString ImageID, QString mask_json_file_path)
{
    QFile mask_json_file(mask_json_file_path);

    if(!mask_json_file.open(QFile::WriteOnly))
    {
        qDebug() << "DataManagerWidget::insertFromMaskJsonFile : open json file failed.";
        return false;
    }

    mask_json_file.resize(0);

    QJsonObject mask_json_Obj = toMaskJsonObj(network_type, ImageID);

    QJsonDocument mask_json_doc = QJsonDocument(mask_json_Obj);

    QByteArray bytes = mask_json_doc.toJson();

    mask_json_file.write(bytes);

    mask_json_file.close();

    return true;
}

void DataManagerWidget::on_Btn_Init_Param_clicked()
{
    QString source_dataset_path_string = ui->textEdit_source_dataset_path->toPlainText();

    if(source_dataset_path_string == "")
    {
        qDebug() << "Source dataset path string is empty.";
        ui->plainTextEdit_msg->appendPlainText("Source dataset path string is empty.");
        return;
    }

    QString target_dataset_path_string = ui->textEdit_target_dataset_path->toPlainText();

    if(target_dataset_path_string == "")
    {
        qDebug() << "Target dataset path string is empty.";
        ui->plainTextEdit_msg->appendPlainText("Target dataset path string is empty.");
        return;
    }

    QString target_dataset_size_string = ui->textEdit_target_dataset_size->toPlainText();
    int target_dataset_size;

    if(target_dataset_size_string != "")
    {
        target_dataset_size = target_dataset_size_string.toInt();
    }
    else
    {
        qDebug() << "Target dataset size is empty.";
        ui->plainTextEdit_msg->appendPlainText("Target dataset size is empty.");
        return;
    }

    QString workers_num_string = ui->textEdit_workers_num->toPlainText();
    int workers_num;

    if(workers_num_string != "")
    {
        workers_num = workers_num_string.toInt();
    }
    else
    {
        qDebug() << "Workers num is empty.";
        ui->plainTextEdit_msg->appendPlainText("Workers num is empty.");
        return;
    }

    data_manager_->init_param(source_dataset_path_string, target_dataset_path_string, target_dataset_size, workers_num);

    qDebug() << "Init Param finished.";
    ui->plainTextEdit_msg->appendPlainText("Init Param finished.");
}

void DataManagerWidget::on_Btn_Create_Work_Station_clicked()
{
    emit signal_CreateWorkStation();
}

void DataManagerWidget::on_Btn_Recycle_Mask_clicked()
{
    emit signal_RecycleMask();
}

void DataManagerWidget::on_Btn_Create_Train_Dataset_clicked()
{
    emit signal_CreateTrainDataset();
}

void DataManagerWidget::on_Btn_Create_Valid_Dataset_clicked()
{
    emit signal_CreateValidDataset();
}

void DataManagerWidget::on_Btn_Recycle_Valid_clicked()
{
    emit signal_RecycleValid();
}

void DataManagerWidget::on_cBox_MaskMode_currentIndexChanged(int index)
{
    if(index < 0 || index >= data_manager_->network_type_list_.size())
    {
        qDebug() << "DataManagerWidget::on_cBox_MaskMode_currentIndexChanged : index out of range.";
        return;
    }

    data_manager_->current_network_type_ = data_manager_->network_type_list_[index];
}

void DataManagerWidget::on_Btn_OpenDir_clicked()
{
    QString load_file_num_for_each_folder_string = ui->lineEdit_LoadFileNumForEachFolder->text();

    if(load_file_num_for_each_folder_string != "")
    {
        int load_file_num_for_each_folder = load_file_num_for_each_folder_string.toInt();

        emit signal_setLoadFileNumForEachFolder(load_file_num_for_each_folder);
    }

    //文件夹路径
    QString openDirPath = QFileDialog::getExistingDirectory(this, "choose src Directory", current_open_dir_path_);

//    QString openDirPath = "D:/chLi/Project/ABACI/FilterSocket/test/bg/";

    if (openDirPath.isEmpty())
    {
        return;
    }

    if(openDirPath.lastIndexOf("/") != openDirPath.size() - 1)
    {
        openDirPath += "/";
    }

    current_open_dir_path_ = openDirPath;

    ui->textEdit_source_dataset_path->setText(openDirPath);

    ui->textEdit_target_dataset_path->setText(openDirPath + "MaskSpace/");

    init(openDirPath);

    QStringList filters;
    filters.append("*.jpg");

    ui->plainTextEdit_msg->appendPlainText("start scanning folder ...");

    emit signal_LoadDir(openDirPath, filters);
}

void DataManagerWidget::on_Btn_QueryMaskAndPredict_clicked()
{
    QString query_row_index_string = ui->lineEdit_QueryMaskAndPredict->text();

    if(query_row_index_string == "")
    {
        qDebug() << "DataManagerWidget::on_Btn_QueryMaskAndPredict_clicked : index input wrong.";
        return;
    }

    int query_row_index = query_row_index_string.toInt() - 1;

    data_manager_->updateMaskAndPredictModel(query_row_index);
}

void DataManagerWidget::on_Btn_InsertFolder_clicked()
{
    QString insert_folder_path = QFileDialog::getExistingDirectory(this, "choose src Directory", current_open_dir_path_);

    if (insert_folder_path.isEmpty())
    {
        qDebug() << "DataManagerWidget::on_Btn_InsertFolder_clicked : no folder selected.";
        return;
    }

    if(insert_folder_path.lastIndexOf("/") != insert_folder_path.size() - 1)
    {
        insert_folder_path += "/";
    }

    current_open_dir_path_ = insert_folder_path;

    emit signal_InsertFolder(insert_folder_path);
}

void DataManagerWidget::on_checkBox_ConnectMask_stateChanged()
{
    data_manager_->connect_mask_ = ui->checkBox_ConnectMask->isChecked();
}

void DataManagerWidget::slot_ShowMsg(QString msg)
{
    ui->plainTextEdit_msg->appendPlainText(msg);
}

void DataManagerWidget::slot_ChangeProgressMaxNum(int max_progress_num)
{
    ui->progressBar->setMaximum(max_progress_num);
}

void DataManagerWidget::slot_ChangeProgressNum(int progress_num)
{
    ui->progressBar->setValue(progress_num);
}
