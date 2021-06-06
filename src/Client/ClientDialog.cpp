#include "ClientDialog.h"

ClientDialog::ClientDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ClientDialog)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::WindowMinimizeButtonHint|Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint);
    this->setWindowTitle(tr("ClientDialog"));
    socket_ = new FilterSocket::Connection(this);
    socket_->setClient(true);
    ui->lineEdit_Port->setValidator(new QIntValidator(0,65536,this));
    connect(socket_,SIGNAL(connected()),this,SLOT(on_socket_connected()));
    connect(socket_,SIGNAL(disconnected()),this,SLOT(on_socket_disconnected()));
    connect(socket_,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(on_socket_error(QAbstractSocket::SocketError)));
    connect(socket_,SIGNAL(processFinished()),this,SLOT(on_process_finished()));
    connect(socket_,SIGNAL(readyRead()),socket_,SLOT(processData()));

    ui->processBtn_lapnet->setEnabled(false);
    ui->processBtn_darknet->setEnabled(false);

    //connect(socket_,SIGNAL(bytesWritten(qint64)),this,SLOT(updateFileProgress(qint64)));

    ui->cBox_SolveMode->addItem("LapNet");
    ui->cBox_SolveMode->addItem("DarkNet");
    ui->cBox_SolveMode->addItem("PolyYolo");
    ui->cBox_SolveMode->setCurrentIndex(1);
    ui->lineEdit_Conda_Env_Name->setText("pytorch");

//    QFile file("Z:/public/MaskTool/src/Config/config.json");

//    if(file.open(QFile::ReadOnly))
//    {
//        QByteArray array_json = file.readAll();

//        file.close();

//        QJsonDocument doc_json = QJsonDocument::fromJson(array_json);
//        QJsonObject obj_json = doc_json.object();

//        QJsonArray label_array = obj_json.value("Area").toObject().value("labelDict").toArray()[0].toObject().value("options").toArray();

//        yolov3_label_list.clear();

//        for(int i = 0; i < label_array.size(); ++i)
//        {
//            yolov3_label_list.append(label_array[i].toArray()[1].toString());
//        }
//    }

//    QFile file("darknet-win-gpu/coco.names");

//    if(file.open(QFile::ReadOnly))
//    {
//        QByteArray array_json = file.readAll();

//        file.close();

//        QStringList label_split_list = QString(array_json).split("\n");

//        yolov3_label_list.clear();

//        for(int i = 0; i < label_split_list.size(); ++i)
//        {
//            yolov3_label_list.append(label_split_list[i]);
//        }
//    }

    yolov3_label_list.clear();

    for(int i = 0; i < 32; ++i)
    {
        yolov3_label_list.append(QString::number(i));
    }

    config_widget = new MaskConfig::ConfigWidget(this);
    ui->HLayout_mask_rect->addWidget(config_widget);
    ui->tabWidget->addTab(config_widget, tr("Mask"));
    config_widget->show();

    mask_widget = new MaskWidget(this);
    ui->HLayout_people_detect_img->addWidget(mask_widget);
    mask_widget->show();
    mask_widget->show_mask = ui->checkBox_ShowMask->isChecked();

    camera_mask_widget = new CameraMaskWidget(this);
    ui->HLayout_camera_mask->addWidget(camera_mask_widget);
    camera_mask_widget->show();
    camera_mask_widget->choose_point_pair = ui->checkBox_ChoosePointPair->isChecked();

    mask_poly_widget = new MaskPolyWidget(this);
    ui->HLayout_mask_poly->addWidget(mask_poly_widget);
    mask_poly_widget->show();

    mask_dataset_control_widget = new MaskDatasetControlWidget(this);
    ui->HLayout_mask_dataset_control->addWidget(mask_dataset_control_widget);
    mask_dataset_control_widget->show();

    data_manager_widget_ = new DataManagerWidget(this);
    ui->HLayout_DataManager->addWidget(data_manager_widget_);
    data_manager_widget_->show();

    //0 : all open
    //1 : mask only
    //2 : 2c test only
    int hide_mode = 0;

    switch(hide_mode)
    {
    case 0:

        ui->tabWidget->removeTab(0);

        break;
    case 1:

        ui->tabWidget->removeTab(6);
        ui->tabWidget->removeTab(5);
        ui->tabWidget->removeTab(4);
        ui->tabWidget->removeTab(3);
        ui->tabWidget->removeTab(2);
        ui->tabWidget->removeTab(1);
        ui->tabWidget->removeTab(0);

        ui->plainTextEdit->hide();
        ui->label_Solve_Mode->hide();
        ui->cBox_SolveMode->hide();
        ui->label_Conda_Env_Name->hide();
        ui->lineEdit_Conda_Env_Name->hide();
        ui->label_Port->hide();
        ui->lineEdit_Port->hide();

        break;
    case 2:

        ui->tabWidget->removeTab(6);
        ui->tabWidget->removeTab(5);
        ui->tabWidget->removeTab(4);
        ui->tabWidget->removeTab(2);
        ui->tabWidget->removeTab(1);
        ui->tabWidget->removeTab(0);

        ui->plainTextEdit->hide();
        ui->label_Solve_Mode->hide();
        ui->cBox_SolveMode->hide();
        ui->label_Conda_Env_Name->hide();
        ui->lineEdit_Conda_Env_Name->hide();
        ui->label_Port->hide();
        ui->lineEdit_Port->hide();

        ui->OpenImgButton->hide();
        ui->processBtn_lapnet->hide();
        ui->processBtn_darknet->hide();
        ui->processBtn_poly_yolo->hide();
        ui->checkBox_LoadNet->hide();
        ui->label_Thresh->hide();
        ui->lineEdit_Thresh->hide();
        ui->Btn_InitModel->hide();
        ui->label_input_img->hide();
        ui->label_output_img->hide();
        ui->progressBar_SendTestFile->hide();

        break;
    }

//    QByteArray array;
//    QJsonDocument doc;
//    QJsonObject obj;
//    obj["a"] = 0;
//    doc.setObject(obj);
//    array.push_back(doc.toJson());

//    QJsonDocument doc_test = QJsonDocument::fromJson(array);
//    QJsonObject obj_test = doc_test.object();

//    qDebug() << obj_test;

    ImageFile image_file("L:/Project/ABACI/FilterSocket/bin_win/dog.jpg");

    qDebug() << image_file.fileName();
    qDebug() << image_file.location();
    qDebug() << image_file.type();
    qDebug() << image_file.width();
    qDebug() << image_file.height();
}

ClientDialog::~ClientDialog()
{
    delete ui;
}

void ClientDialog::connect_to_server()
{
    if(socket_->state() == QAbstractSocket::UnconnectedState)
    {
        on_ConnectServerBtn_clicked();
    }
    else
    {
        on_DisconnectServerBtn_clicked();
        on_ConnectServerBtn_clicked();
    }

    //data_.clear();
}

void ClientDialog::on_OpenImgButton_clicked()
{
     QString filename = QFileDialog::getOpenFileName(0,"","","*.jpg || *.bmp");

     current_img_name = filename;

     if(current_img_name.toStdString() == "")
     {
         return;
     }
     std::cout << current_img_name.toStdString() << std::endl;

     QFile file(current_img_name);

     if(!file.open(QFile::ReadOnly))
     {
         qDebug() << "ClientDialog::on_OpenImgButton_clicked : open file failed.";

         return;
     }

     QByteArray bytes = file.readAll();

     file.close();

     input_img_copy_.loadFromData(bytes);

     QPixmap pixmap;
     pixmap.convertFromImage(input_img_copy_);

     ui->label_input_img->setPixmap(pixmap.scaled(ui->label_input_img->width(), ui->label_input_img->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

     data_.fromQImage(input_img_copy_);

     ui->processBtn_lapnet->setEnabled(true);
     ui->processBtn_darknet->setEnabled(true);
}

void ClientDialog::on_ConnectServerBtn_clicked()
{
    int port = ui->lineEdit_Port->text().toInt();
    if(socket_->state() == QAbstractSocket::UnconnectedState)
    {
        qDebug()<<port;
        socket_->connectToHost("127.0.0.1",port);
        //socket_->connectToHost("117.71.55.48",port);
        //socket_->connectToHost("192.168.0.9",port);
    }
}

void ClientDialog::on_socket_connected()
{
    ui->plainTextEdit->appendPlainText(tr("Socket Connected ") + socket_->localAddress().toString()+ tr(":") + QString::number(socket_->localPort()));
}

void ClientDialog::on_socket_disconnected()
{
    ui->plainTextEdit->appendPlainText(tr("Socket Disconnected ") + socket_->localAddress().toString()+ tr(":") + QString::number(socket_->localPort()));
}

void ClientDialog::on_socket_error(QAbstractSocket::SocketError error)
{
    ui->plainTextEdit->appendPlainText(tr("Socket Error ") + QString::number(error));
}

void ClientDialog::on_DisconnectServerBtn_clicked()
{
    if(socket_->state() != QAbstractSocket::UnconnectedState)
    {
        socket_->disconnectFromHost();
    }
}

void ClientDialog::on_processBtn_lapnet_clicked()
{
    connect_to_server();

    data_.fromQImage(input_img_copy_);

    clock_start_ = clock();

    QJsonObject header_Obj;

    header_Obj["msg_type"] = socket_->Type_Image;
    header_Obj["model_type"] = socket_->Model_LapNet;
    header_Obj["total_size"] = data_.info.img_width_ * data_.info.img_height_ * 3;
    header_Obj["img_width"] = data_.info.img_width_;
    header_Obj["img_height"] = data_.info.img_height_;
    header_Obj["img_format"] = data_.info.img_format_;

    socket_->write(socket_->toHeader(header_Obj) + data_.data_);
}

void ClientDialog::on_processBtn_darknet_clicked()
{
    connect_to_server();

    data_.fromQImage(input_img_copy_);

    clock_start_ = clock();

    QJsonObject header_Obj;

    header_Obj["msg_type"] = socket_->Type_Image;
    header_Obj["model_type"] = socket_->Model_Darknet;
    header_Obj["total_size"] = data_.info.img_width_ * data_.info.img_height_ * 3;
    header_Obj["img_width"] = data_.info.img_width_;
    header_Obj["img_height"] = data_.info.img_height_;
    header_Obj["img_format"] = data_.info.img_format_;

    socket_->write(socket_->toHeader(header_Obj) + data_.data_);
}

void ClientDialog::on_processBtn_poly_yolo_clicked()
{
    connect_to_server();

    data_.fromQImage(input_img_copy_);

    clock_start_ = clock();

    QJsonObject header_Obj;

    header_Obj["msg_type"] = socket_->Type_Image;
    header_Obj["model_type"] = socket_->Model_PolyYolo;
    header_Obj["total_size"] = data_.info.img_width_ * data_.info.img_height_ * 3;
    header_Obj["img_width"] = data_.info.img_width_;
    header_Obj["img_height"] = data_.info.img_height_;
    header_Obj["img_format"] = data_.info.img_format_;

    socket_->write(socket_->toHeader(header_Obj) + data_.data_);
}

void ClientDialog::updateFileProgress(qint64 numBytes)
{
    bytesWritten += int(numBytes);

    std::cout << "progress bar : " << bytesWritten << " , " << data_.info.total_size_ << std::endl;

    int min_num = bytesWritten;

    if(data_.info.total_size_ < min_num)
    {
        min_num = data_.info.total_size_;
    }

    //更新进度条
    this->ui->progressBar_send_obj->setValue(min_num);
}

void ClientDialog::on_SendFile()
{
    QStringList split_list = send_file_srcDirPath.split("/");

    QString base_path = split_list[split_list.size() - 1];

    QString filename = file_list[current_send_file_index].absoluteFilePath();

    QFile outfile(filename);

//        qDebug() << filename << endl;

    filename = base_path + file_list[current_send_file_index].absoluteFilePath().split(send_file_srcDirPath)[1];

//        qDebug() << filename << endl;

    if(!outfile.open(QFile::ReadOnly))
    {
        std::cout << "打开文件失败！" << std::endl;

        return;
    }

    data_.fromQFile(outfile);

    outfile.close();

    connect_to_server();

    QString current_text_model_type = this->ui->cBox_SolveMode->currentText();

    FilterSocket::Connection::ModelType current_model_type;

    if(current_text_model_type == "LapNet")
    {
        current_model_type = socket_->Model_LapNet;
    }
    else if(current_text_model_type == "DarkNet")
    {
        current_model_type = socket_->Model_Darknet;
    }

    int current_text_file_type = this->ui->cBox_folder_type->currentIndex();

    FilterSocket::Connection::FileType current_file_type;

    if(current_text_file_type == 0)
    {
        current_file_type = socket_->File_Dataset;
    }
    else if(current_text_file_type == 1)
    {
        current_file_type = socket_->File_OBJ;
    }

    clock_start_ = clock();

    QJsonObject header_Obj;

    header_Obj["msg_type"] = socket_->Type_File;
    header_Obj["total_size"] = data_.info.total_size_;
    header_Obj["file_name"] = filename;
    header_Obj["model_type"] = current_model_type;
    header_Obj["file_type"] = current_file_type;

    socket_->write(socket_->toHeader(header_Obj) + data_.data_);
}

void ClientDialog::on_SendFolderButton_clicked()
{
    //文件夹路径
    send_file_srcDirPath = QFileDialog::getExistingDirectory(this, "choose src Directory", "/");

    if (send_file_srcDirPath.isEmpty())
    {
        return;
    }
    else
    {
        qDebug() << "srcDirPath=" << send_file_srcDirPath;
    }

    file_list = socket_->GetFileList(send_file_srcDirPath);

    current_send_file_index = 0;

    this->ui->progressBar_send_obj->setMaximum(file_list.size());
    bytesWritten = 0;
    this->ui->progressBar_send_obj->setValue(bytesWritten);

    on_SendFile();
}

void ClientDialog::on_Btn_CreateData_clicked()
{
    connect_to_server();

    QString current_text = this->ui->cBox_SolveMode->currentText();

    FilterSocket::Connection::ModelType current_model_type;

    if(current_text == "LapNet")
    {
        current_model_type = socket_->Model_LapNet;
    }
    else if(current_text == "DarkNet")
    {
        current_model_type = socket_->Model_Darknet;
    }

    qDebug() << "Train Mode : " << current_text << endl;

    QString create_data_num_string = this->ui->textEdit_CreateDataNum->toPlainText();
    QString max_obj_num_per_img_string = this->ui->textEdit_MaxObjNum->toPlainText();

    int create_data_num = 10;
    int max_obj_num_per_img = 5;

    if(create_data_num_string != "")
    {
        create_data_num = create_data_num_string.toInt();
    }
    else
    {
        qDebug() << "ClientDialog::on_Btn_CreateData_clicked : create data num is none.";
    }

    if(max_obj_num_per_img_string != "")
    {
        max_obj_num_per_img = max_obj_num_per_img_string.toInt();
    }
    else
    {
        qDebug() << "ClientDialog::on_Btn_CreateData_clicked : max obj num per img is none.";
    }

    clock_start_ = clock();

    QJsonObject header_Obj;

    header_Obj["msg_type"] = socket_->Type_CreateData;
    header_Obj["total_size"] = 0;
    header_Obj["model_type"] = current_model_type;
    header_Obj["create_data_num"] = create_data_num;
    header_Obj["max_obj_num_per_img"] = max_obj_num_per_img;

    socket_->write(socket_->toHeader(header_Obj) + data_.data_);
}

void ClientDialog::on_Btn_Enhancement_clicked()
{
    connect_to_server();

    QString current_text = this->ui->cBox_SolveMode->currentText();

    FilterSocket::Connection::ModelType current_model_type;

    if(current_text == "LapNet")
    {
        current_model_type = socket_->Model_LapNet;
    }
    else if(current_text == "DarkNet")
    {
        current_model_type = socket_->Model_Darknet;
    }

    qDebug() << "Train Mode : " << current_text << endl;

    QString enhancement_factor_string = this->ui->textEdit_DataEnhancementFactor->toPlainText();

    int enhancement_factor = 10;

    if(enhancement_factor_string != "")
    {
        enhancement_factor = enhancement_factor_string.toInt();
    }

    QString conda_env_name = this->ui->lineEdit_Conda_Env_Name->text();

    clock_start_ = clock();

    QJsonObject header_Obj;

    header_Obj["msg_type"] = socket_->Type_Enhancement;
    header_Obj["total_size"] = 0;
    header_Obj["conda_env_name"] = conda_env_name;
    header_Obj["model_type"] = current_model_type;
    header_Obj["enhancement_factor"] = enhancement_factor;

    socket_->write(socket_->toHeader(header_Obj) + data_.data_);
}

void ClientDialog::on_Btn_DataChange_clicked()
{
    connect_to_server();

    QString current_text = this->ui->cBox_SolveMode->currentText();

    FilterSocket::Connection::ModelType current_model_type;

    if(current_text == "LapNet")
    {
        current_model_type = socket_->Model_LapNet;
    }
    else if(current_text == "DarkNet")
    {
        current_model_type = socket_->Model_Darknet;
    }

    qDebug() << "Train Mode : " << current_text << endl;

    QString conda_env_name = this->ui->lineEdit_Conda_Env_Name->text();

    QString label_num_string = this->ui->lineEdit_LabelNum->text();

    int label_num = 80;

    if(label_num_string != "")
    {
        label_num = label_num_string.toInt();
    }

    QString train_percent_string = this->ui->lineEdit_TrainPercent->text();

    int train_percent = 80;

    if(train_percent_string != "")
    {
        train_percent = train_percent_string.toInt();
    }

    clock_start_ = clock();

    QJsonObject header_Obj;

    header_Obj["msg_type"] = socket_->Type_DataChange;
    header_Obj["total_size"] = 0;
    header_Obj["conda_env_name"] = conda_env_name;
    header_Obj["train_percent"] = train_percent;
    header_Obj["label_num"] = label_num;
    header_Obj["model_type"] = current_model_type;

    socket_->write(socket_->toHeader(header_Obj) + data_.data_);
}

void ClientDialog::on_Btn_Train_clicked()
{
    connect_to_server();

    QString current_text = this->ui->cBox_SolveMode->currentText();

    FilterSocket::Connection::ModelType current_model_type;

    if(current_text == "LapNet")
    {
        current_model_type = socket_->Model_LapNet;
    }
    else if(current_text == "DarkNet")
    {
        current_model_type = socket_->Model_Darknet;
    }

    qDebug() << "Train Mode : " << current_text << endl;

    QString conda_env_name = this->ui->lineEdit_Conda_Env_Name->text();

    clock_start_ = clock();

    QJsonObject header_Obj;

    header_Obj["msg_type"] = socket_->Type_Train;
    header_Obj["total_size"] = 0;
    header_Obj["conda_env_name"] = conda_env_name;
    header_Obj["model_type"] = current_model_type;

    socket_->write(socket_->toHeader(header_Obj) + data_.data_);
}

void ClientDialog::on_Btn_StopTrain_clicked()
{
    connect_to_server();

    QString current_text = this->ui->cBox_SolveMode->currentText();

    FilterSocket::Connection::ModelType current_model_type;

    if(current_text == "LapNet")
    {
        current_model_type = socket_->Model_LapNet;
    }
    else if(current_text == "DarkNet")
    {
        current_model_type = socket_->Model_Darknet;
    }

    qDebug() << "Train Mode : " << current_text << endl;

    clock_start_ = clock();

    QJsonObject header_Obj;

    header_Obj["msg_type"] = socket_->Type_StopTrain;
    header_Obj["total_size"] = 0;
    header_Obj["model_type"] = current_model_type;

    socket_->write(socket_->toHeader(header_Obj) + data_.data_);
}

void ClientDialog::on_Btn_Valid_clicked()
{
    data_.clear();

    connect_to_server();

    QString current_text = this->ui->cBox_SolveMode->currentText();

    FilterSocket::Connection::ModelType current_model_type;

    if(current_text == "LapNet")
    {
        current_model_type = socket_->Model_LapNet;
    }
    else if(current_text == "DarkNet")
    {
        current_model_type = socket_->Model_Darknet;
    }

    qDebug() << "Train Mode : " << current_text << endl;

    QString conda_env_name = this->ui->lineEdit_Conda_Env_Name->text();

    clock_start_ = clock();

    QJsonObject header_Obj;

    header_Obj["msg_type"] = socket_->Type_Valid;
    header_Obj["total_size"] = 0;
    header_Obj["conda_env_name"] = conda_env_name;
    header_Obj["model_type"] = current_model_type;

    socket_->write(socket_->toHeader(header_Obj) + data_.data_);
}

void ClientDialog::on_Btn_GetResult_clicked()
{
    connect_to_server();

    QString return_result_folder_name_string = this->ui->lineEdit_ReturnResultFolderName->text();

    QString current_text = this->ui->cBox_SolveMode->currentText();

    FilterSocket::Connection::ModelType current_model_type;

    if(current_text == "LapNet")
    {
        current_model_type = socket_->Model_LapNet;
    }
    else if(current_text == "DarkNet")
    {
        current_model_type = socket_->Model_Darknet;
    }

    qDebug() << "Train Mode : " << current_text << endl;

    if(return_result_folder_name_string == "")
    {
        QDir dir_(QDir::currentPath() + "/../Client_DataBase/");

        int current_idx = 1;

        QFileInfoList file_info_list = dir_.entryInfoList(QDir::Dirs | QDir::NoSymLinks);

        return_result_folder_name_string = "return_result_" + current_text + "_" + QString::number(current_idx);
    }

    clock_start_ = clock();

    this->ui->progressBar_send_obj->setMaximum(1);
    bytesWritten = 0;
    this->ui->progressBar_send_obj->setValue(bytesWritten);

    QJsonObject header_Obj;

    header_Obj["msg_type"] = socket_->Type_ReturnResult;
    header_Obj["total_size"] = 0;
    header_Obj["model_type"] = current_model_type;

    socket_->write(socket_->toHeader(header_Obj) + data_.data_);
}

void ClientDialog::on_Btn_SetResult_clicked()
{

}

void ClientDialog::on_SendTestFile()
{
    qDebug() << current_send_file_index << "/" << file_list.size();
    if(current_send_file_index >= file_list.size())
    {
        bytesWritten = file_list.size();
        this->ui->progressBar_SendTestFile->setValue(bytesWritten);

        qDebug() << "ClientDialog::on_SendTestFile : img send finished.";

        return;
    }

    QStringList split_list = send_file_srcDirPath.split("/");

    QString base_path = split_list[split_list.size() - 1];

    QString filename = file_list[current_send_file_index].absoluteFilePath();

    if(filename.split(".")[1] != "jpg")
    {
        ++current_send_file_index;
        ++bytesWritten;
        this->ui->progressBar_SendTestFile->setValue(bytesWritten);

        on_SendTestFile();
    }
    else
    {
        qDebug() << filename;

        QImage outimage(filename);

        input_img_copy_ = outimage.copy();

        data_.fromQImage(outimage);

        connect_to_server();

        QString current_text_model_type = this->ui->cBox_SolveMode->currentText();

        FilterSocket::Connection::ModelType current_model_type;

        if(current_text_model_type == "LapNet")
        {
            current_model_type = socket_->Model_LapNet;
        }
        else if(current_text_model_type == "DarkNet")
        {
            current_model_type = socket_->Model_Darknet;
        }

        int current_text_file_type = this->ui->cBox_folder_type->currentIndex();

        FilterSocket::Connection::FileType current_file_type;

        if(current_text_file_type == 0)
        {
            current_file_type = socket_->File_Dataset;
        }
        else if(current_text_file_type == 1)
        {
            current_file_type = socket_->File_OBJ;
        }

        clock_start_ = clock();

        QJsonObject header_Obj;

        header_Obj["msg_type"] = socket_->Type_ImageList;
        header_Obj["img_width"] = data_.info.img_width_;
        header_Obj["img_height"] = data_.info.img_height_;
        header_Obj["img_format"] = data_.info.img_format_;
        header_Obj["total_size"] = data_.info.img_width_ * data_.info.img_height_ * 3;
        header_Obj["model_type"] = socket_->Model_Darknet;

        socket_->write(socket_->toHeader(header_Obj) + data_.data_);
    }
}

void ClientDialog::on_Btn_DarknetTest_clicked()
{
//    //文件夹路径
//    send_file_srcDirPath = QFileDialog::getExistingDirectory(this, "choose src Directory", "/");

//    if (send_file_srcDirPath.isEmpty())
//    {
//        return;
//    }
//    else
//    {
//        qDebug() << "srcDirPath=" << send_file_srcDirPath;
//    }

//    file_list = socket_->GetFileList(send_file_srcDirPath);

//    current_send_file_index = 0;

//    this->ui->progressBar_SendTestFile->setMaximum(file_list.size());
//    bytesWritten = 0;
//    this->ui->progressBar_SendTestFile->setValue(bytesWritten);

//    on_SendTestFile();

    QString ImageDir = ui->lineEdit_ImageDir->text();
    QString OutputDir = ui->lineEdit_OutputDir->text();

    if(ImageDir == "")
    {
        qDebug() << "ClientDialog::on_Btn_DarknetTest_clicked : please select ImageDir first.";
        return;
    }
    if(OutputDir == "")
    {
        qDebug() << "ClientDialog::on_Btn_DarknetTest_clicked : please select OutputDir first.";
        return;
    }

    connect_to_server();

    clock_start_ = clock();

    QJsonObject header_Obj;

    header_Obj["msg_type"] = socket_->Type_Test;
    header_Obj["total_size"] = 0;
    header_Obj["model_type"] = socket_->Model_Darknet;
    header_Obj["image_dir"] = ImageDir;
    header_Obj["output_dir"] = OutputDir;

    socket_->write(socket_->toHeader(header_Obj) + data_.data_);
}

void ClientDialog::on_Btn_GetReceivedMessage_clicked()
{
    connect_to_server();

    clock_start_ = clock();

    QJsonObject header_Obj;

    header_Obj["msg_type"] = socket_->Type_ReceivedFileMsg;
    header_Obj["total_size"] = 0;
    header_Obj["model_type"] = socket_->Model_Free;

    socket_->write(socket_->toHeader(header_Obj) + data_.data_);
}

void ClientDialog::on_Btn_ClearAll_clicked()
{
    connect_to_server();

    clock_start_ = clock();

    QJsonObject header_Obj;

    header_Obj["msg_type"] = socket_->Type_ClearAll;
    header_Obj["total_size"] = 0;
    header_Obj["model_type"] = socket_->Model_Free;

    socket_->write(socket_->toHeader(header_Obj) + data_.data_);
}

void ClientDialog::on_Btn_InitModel_clicked()
{
    connect_to_server();

    QString conda_env_name = this->ui->lineEdit_Conda_Env_Name->text();

    bool load_net = this->ui->checkBox_LoadNet->isEnabled();

    double thresh = 0.5;

    clock_start_ = clock();

    QJsonObject header_Obj;

    header_Obj["msg_type"] = socket_->Type_InitModel;
    header_Obj["total_size"] = 0;
    header_Obj["conda_env_name"] = conda_env_name;
    header_Obj["load_net"] = load_net;
    header_Obj["thresh"] = thresh;
    header_Obj["model_type"] = socket_->Model_Free;

//    socket_->write(socket_->toInitModelHeader(conda_env_name, ) + data_.data_);
}

void ClientDialog::on_Frame_send()
{
    connect_to_server();

    data_.clear();

    QString conda_env_name = this->ui->lineEdit_Conda_Env_Name->text();

    clock_start_ = clock();

    QJsonObject header_Obj;

    header_Obj["msg_type"] = socket_->Type_Frame;
    header_Obj["conda_env_name"] = conda_env_name;
    header_Obj["total_size"] = 0;
    header_Obj["model_type"] = socket_->Model_Darknet;

    socket_->write(socket_->toHeader(header_Obj) + data_.data_);
}

void ClientDialog::on_FrameBBox_get()
{
    connect_to_server();

    data_.clear();

    clock_start_ = clock();

    QJsonObject header_Obj;

    header_Obj["msg_type"] = socket_->Type_FrameBBox;
    header_Obj["bbox_num"] = 0;
    header_Obj["total_size"] = 0;
    header_Obj["model_type"] = socket_->Model_Darknet;

    socket_->write(socket_->toHeader(header_Obj) + data_.data_);
}

void ClientDialog::on_Video_send()
{
    connect_to_server();

    data_.clear();

    QString conda_env_name = this->ui->lineEdit_Conda_Env_Name->text();

    clock_start_ = clock();

    QJsonObject header_Obj;

    header_Obj["msg_type"] = socket_->Type_Video;
    header_Obj["total_size"] = 0;
    header_Obj["conda_env_name"] = conda_env_name;
    header_Obj["model_type"] = socket_->Model_Darknet;

    socket_->write(socket_->toHeader(header_Obj) + data_.data_);
}

void ClientDialog::on_VideoBBox_get()
{
    connect_to_server();

    data_.clear();

    clock_start_ = clock();

    QJsonObject header_Obj;

    header_Obj["msg_type"] = socket_->Type_VideoBBox;
    header_Obj["bbox_num"] = 0;
    header_Obj["total_size"] = 0;
    header_Obj["model_type"] = socket_->Model_Darknet;

    socket_->write(socket_->toHeader(header_Obj) + data_.data_);
}

void ClientDialog::on_Btn_GetCameraFrame_clicked()
{
    video_mode_ = false;

    on_Frame_send();
}

void ClientDialog::on_Btn_InitCamera_clicked()
{
    camera_mask_widget->init_camera();

    if(camera_mask_widget->init_ok)
    {
        QString fixed_dist_string = ui->lineEdit_DetectDensity->text();

        if(fixed_dist_string != "")
        {
            camera_mask_widget->cut_Draw_Background_By_Fixed_Dist(fixed_dist_string.toInt());
        }
        else
        {
            camera_mask_widget->cut_Draw_Background_By_Fixed_Dist(40);
        }

        camera_mask_widget->update();
    }
}

void ClientDialog::on_Btn_CameraCap_clicked()
{
    video_mode_ = true;

    on_Video_send();
}

void ClientDialog::on_Btn_StopCameraCap_clicked()
{
    connect_to_server();

    data_.clear();

    clock_start_ = clock();

    QJsonObject header_Obj;

    header_Obj["msg_type"] = socket_->Type_StopVideo;
    header_Obj["total_size"] = 0;
    header_Obj["model_type"] = socket_->Model_Darknet;

    socket_->write(socket_->toHeader(header_Obj) + data_.data_);
}

void ClientDialog::on_Btn_SelectImageDir_clicked()
{
    //文件夹路径
    QString image_dir = QFileDialog::getExistingDirectory(this, "choose src Directory", "/");

    if (image_dir.isEmpty())
    {
        return;
    }
    else
    {
        qDebug() << "ImageDir=" << image_dir;
    }

    if(image_dir.lastIndexOf("/") != image_dir.size() - 1)
    {
        image_dir += "/";
    }

    ui->lineEdit_ImageDir->setText(image_dir);
}

void ClientDialog::on_Btn_SelectOutputDir_clicked()
{
    //文件夹路径
    QString output_dir = QFileDialog::getExistingDirectory(this, "choose src Directory", "/");

    if (output_dir.isEmpty())
    {
        return;
    }
    else
    {
        qDebug() << "OutputDir=" << output_dir;
    }

    if(output_dir.lastIndexOf("/") != output_dir.size() - 1)
    {
        output_dir += "/";
    }

    ui->lineEdit_OutputDir->setText(output_dir);
}

void ClientDialog::on_checkBox_ShowMask_stateChanged(int state)
{
    bool is_show_mask = ui->checkBox_ShowMask->isChecked();

    this->mask_widget->show_mask = is_show_mask;

    this->mask_widget->update();

    qDebug() << "Change show mask mode to state : " << is_show_mask << endl;
}

void ClientDialog::on_checkBox_SaveCap_stateChanged(int state)
{
    bool is_save_cap = ui->checkBox_ShowMask->isChecked();

    save_camera_cap_ = is_save_cap;

    qDebug() << "Change save cap mode to state : " << is_save_cap << endl;
}

void ClientDialog::on_checkBox_ChoosePointPair_stateChanged(int state)
{
    bool is_choose_point_pair = ui->checkBox_ChoosePointPair->isChecked();

    this->camera_mask_widget->choose_point_pair = is_choose_point_pair;

    this->camera_mask_widget->update();

    qDebug() << "Change choose point pair mode to state : " << is_choose_point_pair << endl;
}

void ClientDialog::on_cBox_SolveMode_currentIndexChanged(int index)
{
//    qDebug() << "Change solve mode to index : " << index << endl;
}

void ClientDialog::on_cBox_folder_type_currentIndexChanged(int index)
{
//    qDebug() << "Change folder type to index : " << index << endl;
}

void ClientDialog::on_process_finished()
{
    clock_t t1 = clock();
#ifdef WIN32
    ui->plainTextEdit->appendPlainText(tr("Cost time: ") + QString::number(t1 - clock_start_) + tr("ms"));
#endif
#ifdef Linux
    ui->plainTextEdit->appendPlainText(tr("Cost time: ") + QString::number(int((t1 - clock_start_)/100)) + tr("ms"));
#endif
    FilterSocket::Connection::ConnectedData &res = socket_->getData();

//    res.data_ = QByteArray::fromBase64(res.data_);

    if(res.info.model_type_ == socket_->Model_LapNet)
    {
        if(res.info.message_type_ == socket_->Type_Image)
        {
            QImage img;

            res.toQImage(img);

            QPixmap pixmap = QPixmap::fromImage(img);

            ui->label_input_img->setPixmap(QPixmap::fromImage(input_img_copy_).scaled(ui->label_input_img->width(), ui->label_input_img->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
            ui->label_output_img->setPixmap(pixmap.scaled(ui->label_output_img->width(), ui->label_output_img->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
        }
        else if(res.info.message_type_ == socket_->Type_ImageList)
        {
            QImage img;

            res.toQImage(img);

            QPixmap pixmap = QPixmap::fromImage(img);

            ui->label_input_img->setPixmap(QPixmap::fromImage(input_img_copy_).scaled(ui->label_input_img->width(), ui->label_input_img->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
            ui->label_output_img->setPixmap(pixmap.scaled(ui->label_output_img->width(), ui->label_output_img->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

            ++current_send_file_index;

            ++bytesWritten;
            this->ui->progressBar_send_obj->setValue(bytesWritten);

            if(current_send_file_index < file_list.size())
            {
                on_SendTestFile();
            }
        }
        else if(res.info.message_type_ == socket_->Type_File)
        {
            if(res.info.file_name_.contains('/'))
            {
                QString folder_path = QDir::currentPath() + "/../Client_DataBase/received_model/lapnet_model/" + res.info.file_name_.left(res.info.file_name_.lastIndexOf('/'));

                QDir dir(folder_path);

                if(!dir.exists())
                {
                    dir.mkpath(folder_path);
                }
            }

            QFile file_recieved(QDir::currentPath() + "/../Client_DataBase/received_model/lapnet_model/" + res.info.file_name_);

            if(!file_recieved.open(QFile::WriteOnly))
            {
                qDebug() << "Couldn't write---1" << "\n";
            }

            res.toQFile(file_recieved);

            file_recieved.close();

            this->ui->progressBar_send_obj->setMaximum(1);
            bytesWritten = 1;
            this->ui->progressBar_send_obj->setValue(bytesWritten);

            qDebug() << "Finished recieve---1" << "\n";
        }
        else if(res.info.message_type_ == socket_->Type_FileFinished)
        {
            ++current_send_file_index;

            bytesWritten += 1;
            this->ui->progressBar_send_obj->setValue(bytesWritten);

            if(current_send_file_index < file_list.size())
            {
                on_SendFile();
            }
        }
    }
    else if(res.info.model_type_ == socket_->Model_Darknet)
    {
        if(res.info.message_type_ == socket_->Type_BBox)
        {
            QString bbox_text;

            bbox_text += tr("bbox size : ") + QString::number(res.info.bbox_num_) + "\n";

            for(int i = 0; i < res.info.bbox_num_; ++i)
            {
                bbox_text += QString::fromStdString(std::to_string(res.getBBoxLabel(i)) + " : ");
                bbox_text += QString::fromStdString(std::to_string(res.getBBoxX(i)) + ",");
                bbox_text += QString::fromStdString(std::to_string(res.getBBoxY(i)) + ",");
                bbox_text += QString::fromStdString(std::to_string(res.getBBoxW(i)) + ",");
                bbox_text += QString::fromStdString(std::to_string(res.getBBoxH(i)) + "\n");
            }

            ui->plainTextEdit->appendPlainText(bbox_text);

            QImage result_image = input_img_copy_.copy();

            QPainter painter(&result_image);

            painter.setCompositionMode(QPainter::CompositionMode_SourceIn);

            QPen pen = painter.pen();
            pen.setColor(Qt::red);
            QFont font = painter.font();
            font.setBold(true);
            font.setPixelSize(30);
            painter.setPen(pen);
            painter.setFont(font);

            std::vector<std::vector<int>> bbox_result = res.toBBox();

            int line_width = 3;

            for(int i = 0; i < bbox_result.size(); ++i)
            {
                painter.drawText(bbox_result[i][0], bbox_result[i][1], yolov3_label_list[res.getBBoxLabel(i)]);

                for(int j = 0; j < bbox_result[i][2]; ++j)
                {
                    for(int k = 0; k <= line_width; ++k)
                    {
                        result_image.setPixel(bbox_result[i][0] + j, bbox_result[i][1] + k, qRgb(255, 0, 0));
                        result_image.setPixel(bbox_result[i][0] + j, bbox_result[i][1] - k, qRgb(255, 0, 0));

                        result_image.setPixel(bbox_result[i][0] + j, bbox_result[i][1] + bbox_result[i][3] + k, qRgb(255, 0, 0));
                        result_image.setPixel(bbox_result[i][0] + j, bbox_result[i][1] + bbox_result[i][3] - k, qRgb(255, 0, 0));
                    }
                }

                for(int j = 0; j < bbox_result[i][3]; ++j)
                {
                    for(int k = 0; k <= line_width; ++k)
                    {
                        result_image.setPixel(bbox_result[i][0] + k, bbox_result[i][1] + j, qRgb(255, 0, 0));
                        result_image.setPixel(bbox_result[i][0] - k, bbox_result[i][1] + j, qRgb(255, 0, 0));

                        result_image.setPixel(bbox_result[i][0] + bbox_result[i][2] + k, bbox_result[i][1] + j, qRgb(255, 0, 0));
                        result_image.setPixel(bbox_result[i][0] + bbox_result[i][2] - k, bbox_result[i][1] + j, qRgb(255, 0, 0));
                    }
                }
            }

            ui->label_input_img->setPixmap(QPixmap::fromImage(input_img_copy_).scaled(ui->label_input_img->width(), ui->label_input_img->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
            ui->label_output_img->setPixmap(QPixmap::fromImage(result_image).scaled(ui->label_output_img->width(), ui->label_output_img->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

            result_image.save("test_out.jpg");
        }
        else if(res.info.message_type_ == socket_->Type_BBoxList)
        {
            QString bbox_text;

            bbox_text += tr("bbox size : ") + QString::number(res.info.bbox_num_) + "\n";

            for(int i = 0; i < res.info.bbox_num_; ++i)
            {
                bbox_text += QString::fromStdString(std::to_string(res.getBBoxLabel(i)) + " : ");
                bbox_text += QString::fromStdString(std::to_string(res.getBBoxX(i)) + ",");
                bbox_text += QString::fromStdString(std::to_string(res.getBBoxY(i)) + ",");
                bbox_text += QString::fromStdString(std::to_string(res.getBBoxW(i)) + ",");
                bbox_text += QString::fromStdString(std::to_string(res.getBBoxH(i)) + "\n");
            }

            ui->plainTextEdit->appendPlainText(bbox_text);

            QImage result_image = input_img_copy_.copy();

            QPainter painter(&result_image);

            painter.setCompositionMode(QPainter::CompositionMode_SourceIn);

            QPen pen = painter.pen();
            pen.setColor(Qt::red);
            QFont font = painter.font();
            font.setBold(true);
            font.setPixelSize(70);
            painter.setPen(pen);
            painter.setFont(font);

            std::vector<std::vector<int>> bbox_result = res.toBBox();

            int line_width = 3;

            for(int i = 0; i < bbox_result.size(); ++i)
            {
                painter.drawText(bbox_result[i][0], bbox_result[i][1], yolov3_label_list[res.getBBoxLabel(i)]);

                for(int j = 0; j < bbox_result[i][2]; ++j)
                {
                    for(int k = 0; k <= line_width; ++k)
                    {
                        result_image.setPixel(bbox_result[i][0] + j, bbox_result[i][1] + k, qRgb(255, 0, 0));
                        result_image.setPixel(bbox_result[i][0] + j, bbox_result[i][1] - k, qRgb(255, 0, 0));

                        result_image.setPixel(bbox_result[i][0] + j, bbox_result[i][1] + bbox_result[i][3] + k, qRgb(255, 0, 0));
                        result_image.setPixel(bbox_result[i][0] + j, bbox_result[i][1] + bbox_result[i][3] - k, qRgb(255, 0, 0));
                    }
                }

                for(int j = 0; j < bbox_result[i][3]; ++j)
                {
                    for(int k = 0; k <= line_width; ++k)
                    {
                        result_image.setPixel(bbox_result[i][0] + k, bbox_result[i][1] + j, qRgb(255, 0, 0));
                        result_image.setPixel(bbox_result[i][0] - k, bbox_result[i][1] + j, qRgb(255, 0, 0));

                        result_image.setPixel(bbox_result[i][0] + bbox_result[i][2] + k, bbox_result[i][1] + j, qRgb(255, 0, 0));
                        result_image.setPixel(bbox_result[i][0] + bbox_result[i][2] - k, bbox_result[i][1] + j, qRgb(255, 0, 0));
                    }
                }
            }

            ui->label_input_img->setPixmap(QPixmap::fromImage(input_img_copy_).scaled(ui->label_input_img->width(), ui->label_input_img->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
            ui->label_output_img->setPixmap(QPixmap::fromImage(result_image).scaled(ui->label_output_img->width(), ui->label_output_img->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

            ++current_send_file_index;

            ++bytesWritten;
            this->ui->progressBar_send_obj->setValue(bytesWritten);

            if(current_send_file_index < file_list.size())
            {
                on_SendTestFile();
            }
        }
        else if(res.info.message_type_ == socket_->Type_File)
        {
            if(res.info.file_name_.contains('/'))
            {
                QString folder_path = QDir::currentPath() + "/../Client_DataBase/received_model/darknet_model/" + res.info.file_name_.left(res.info.file_name_.lastIndexOf('/'));

                QDir dir(folder_path);

                if(!dir.exists())
                {
                    dir.mkpath(folder_path);
                }
            }

            QFile file_recieved(QDir::currentPath() + "/../Client_DataBase/received_model/darknet_model/" + res.info.file_name_);

            if(!file_recieved.open(QFile::WriteOnly))
            {
                qDebug() << "Couldn't write---1" << "\n";
            }

            res.toQFile(file_recieved);

            file_recieved.close();

            this->ui->progressBar_send_obj->setMaximum(1);
            bytesWritten = 1;
            this->ui->progressBar_send_obj->setValue(bytesWritten);

            qDebug() << "Finished recieve---1" << "\n";
        }
        else if(res.info.message_type_ == socket_->Type_FileFinished)
        {
            ++current_send_file_index;

            ++bytesWritten;
            this->ui->progressBar_send_obj->setValue(bytesWritten);

            if(current_send_file_index < file_list.size())
            {
                on_SendFile();
            }
        }
        else if(res.info.message_type_ == socket_->Type_Image)
        {
            res.toQImage(camera_img_copy_);

            qDebug() << "Finished recieve video frame---1" << "\n";

            if(video_mode_)
            {
                on_VideoBBox_get();
            }
            else
            {
                on_FrameBBox_get();
            }
        }
        else if(res.info.message_type_ == socket_->Type_FrameBBox)
        {
            QString bbox_text;

            bbox_text += tr("bbox size : ") + QString::number(res.info.bbox_num_) + "\n";

            for(int i = 0; i < res.info.bbox_num_; ++i)
            {
                bbox_text += QString::fromStdString(std::to_string(res.getBBoxLabel(i)) + " : ");
                bbox_text += QString::fromStdString(std::to_string(res.getBBoxX(i)) + ",");
                bbox_text += QString::fromStdString(std::to_string(res.getBBoxY(i)) + ",");
                bbox_text += QString::fromStdString(std::to_string(res.getBBoxW(i)) + ",");
                bbox_text += QString::fromStdString(std::to_string(res.getBBoxH(i)) + "\n");
            }

            ui->plainTextEdit->appendPlainText(bbox_text);

            QImage result_image = camera_img_copy_.copy();

            QPainter painter(&result_image);

            painter.setCompositionMode(QPainter::CompositionMode_SourceIn);

            QPen pen = painter.pen();
            pen.setColor(Qt::red);
            QFont font = painter.font();
            font.setBold(true);
            font.setPixelSize(30);
            painter.setPen(pen);
            painter.setFont(font);

            std::vector<std::vector<int>> bbox_result = res.toBBox();

            int line_width = 3;

            for(int i = 0; i < bbox_result.size(); ++i)
            {
                if(bbox_result[i][4] == 0 && bbox_result[i][1] > 100 && bbox_result[i][1] < 150 && bbox_result[i][2] > 30 && bbox_result[i][2] < 60 && bbox_result[i][3] > 80 && bbox_result[i][3] < 150)
                {
                    continue;
                }
                painter.drawText(bbox_result[i][0], bbox_result[i][1], yolov3_label_list[res.getBBoxLabel(i)]);

                for(int j = 0; j < bbox_result[i][2]; ++j)
                {
                    for(int k = 0; k <= line_width; ++k)
                    {
                        result_image.setPixel(bbox_result[i][0] + j, bbox_result[i][1] + k, qRgb(255, 0, 0));
                        result_image.setPixel(bbox_result[i][0] + j, bbox_result[i][1] - k, qRgb(255, 0, 0));

                        result_image.setPixel(bbox_result[i][0] + j, bbox_result[i][1] + bbox_result[i][3] + k, qRgb(255, 0, 0));
                        result_image.setPixel(bbox_result[i][0] + j, bbox_result[i][1] + bbox_result[i][3] - k, qRgb(255, 0, 0));
                    }
                }

                for(int j = 0; j < bbox_result[i][3]; ++j)
                {
                    for(int k = 0; k <= line_width; ++k)
                    {
                        result_image.setPixel(bbox_result[i][0] + k, bbox_result[i][1] + j, qRgb(255, 0, 0));
                        result_image.setPixel(bbox_result[i][0] - k, bbox_result[i][1] + j, qRgb(255, 0, 0));

                        result_image.setPixel(bbox_result[i][0] + bbox_result[i][2] + k, bbox_result[i][1] + j, qRgb(255, 0, 0));
                        result_image.setPixel(bbox_result[i][0] + bbox_result[i][2] - k, bbox_result[i][1] + j, qRgb(255, 0, 0));
                    }
                }
            }

            camera_mask_widget->set_background(result_image);
        }
        else if(res.info.message_type_ == socket_->Type_VideoBBox)
        {
            QString bbox_text;

            bbox_text += tr("bbox size : ") + QString::number(res.info.bbox_num_) + "\n";

            for(int i = 0; i < res.info.bbox_num_; ++i)
            {
                bbox_text += QString::fromStdString(std::to_string(res.getBBoxLabel(i)) + " : ");
                bbox_text += QString::fromStdString(std::to_string(res.getBBoxX(i)) + ",");
                bbox_text += QString::fromStdString(std::to_string(res.getBBoxY(i)) + ",");
                bbox_text += QString::fromStdString(std::to_string(res.getBBoxW(i)) + ",");
                bbox_text += QString::fromStdString(std::to_string(res.getBBoxH(i)) + "\n");
            }

            ui->plainTextEdit->appendPlainText(bbox_text);

            QImage result_image = camera_img_copy_.copy();

            QPainter painter(&result_image);

            painter.setCompositionMode(QPainter::CompositionMode_SourceIn);

            QPen pen = painter.pen();
            pen.setColor(Qt::red);
            QFont font = painter.font();
            font.setBold(true);
            font.setPixelSize(30);
            painter.setPen(pen);
            painter.setFont(font);

            std::vector<std::vector<int>> bbox_result = res.toBBox();

            int line_width = 3;

            for(int i = 0; i < bbox_result.size(); ++i)
            {
                if(bbox_result[i][4] == 0 && bbox_result[i][1] > 100 && bbox_result[i][1] < 150 && bbox_result[i][2] > 30 && bbox_result[i][2] < 60 && bbox_result[i][3] > 80 && bbox_result[i][3] < 150)
                {
                    continue;
                }

                if(bbox_result[i][4] == 0 && camera_mask_widget->init_ok)
                {
                    camera_mask_widget->add_Detect_Position(bbox_result[i][0] + int(bbox_result[i][2] / 2), bbox_result[i][1] + bbox_result[i][3]);
                }

                painter.drawText(bbox_result[i][0], bbox_result[i][1], yolov3_label_list[res.getBBoxLabel(i)]);

                for(int j = 0; j < bbox_result[i][2]; ++j)
                {
                    for(int k = 0; k <= line_width; ++k)
                    {
                        result_image.setPixel(bbox_result[i][0] + j, bbox_result[i][1] + k, qRgb(255, 0, 0));
                        result_image.setPixel(bbox_result[i][0] + j, bbox_result[i][1] - k, qRgb(255, 0, 0));

                        result_image.setPixel(bbox_result[i][0] + j, bbox_result[i][1] + bbox_result[i][3] + k, qRgb(255, 0, 0));
                        result_image.setPixel(bbox_result[i][0] + j, bbox_result[i][1] + bbox_result[i][3] - k, qRgb(255, 0, 0));
                    }
                }

                for(int j = 0; j < bbox_result[i][3]; ++j)
                {
                    for(int k = 0; k <= line_width; ++k)
                    {
                        result_image.setPixel(bbox_result[i][0] + k, bbox_result[i][1] + j, qRgb(255, 0, 0));
                        result_image.setPixel(bbox_result[i][0] - k, bbox_result[i][1] + j, qRgb(255, 0, 0));

                        result_image.setPixel(bbox_result[i][0] + bbox_result[i][2] + k, bbox_result[i][1] + j, qRgb(255, 0, 0));
                        result_image.setPixel(bbox_result[i][0] + bbox_result[i][2] - k, bbox_result[i][1] + j, qRgb(255, 0, 0));
                    }
                }
            }

            mask_widget->set_background(result_image);
            camera_mask_widget->set_background(result_image);

            //ui->label_people_detect_img->setPixmap(QPixmap::fromImage(result_image).scaled(ui->label_people_detect_img->width(), ui->label_people_detect_img->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

            on_Video_send();
        }
    }
    else if(res.info.model_type_ == socket_->Model_Free)
    {
        if(res.info.message_type_ == socket_->Type_File)
        {
            if(res.info.file_type_ == socket_->File_ReceivedFileMsg)
            {
                QDir dir(res.info.file_name_.left(res.info.file_name_.lastIndexOf("/")));

                if(!dir.exists())
                {
                    dir.mkdir(dir.absolutePath());
                }

                QFile file_recieved(QDir::currentPath() + "/" + res.info.file_name_);

                if(!file_recieved.open(QFile::WriteOnly))
                {
                    qDebug() << "Couldn't write---1" << "\n";
                }

                res.toQFile(file_recieved);

                file_recieved.close();

                this->ui->progressBar_send_obj->setMaximum(1);
                bytesWritten = 1;
                this->ui->progressBar_send_obj->setValue(bytesWritten);

                qDebug() << "Finished recieve---1" << "\n";

                QFile file_recieved_read(QDir::currentPath() + "/" + res.info.file_name_);

                if(!file_recieved_read.open(QFile::ReadOnly))
                {
                    qDebug() << "Couldn't read---1" << "\n";
                }

                this->ui->plainTextEdit_received_output->setPlainText(file_recieved_read.readAll());

                file_recieved_read.close();
            }
        }
    }

    //on_DisconnectServerBtn_clicked();
}
