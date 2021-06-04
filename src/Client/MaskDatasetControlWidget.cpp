#include "MaskDatasetControlWidget.h"
#include "ui_maskdatasetcontrolwidget.h"

MaskDatasetControlWidget::MaskDatasetControlWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MaskDatasetControlWidget)
{
    ui->setupUi(this);

    debug_skip_num_ = 1;

    get_file_list_finished_ = false;
}

MaskDatasetControlWidget::~MaskDatasetControlWidget()
{
    delete ui;
}

void MaskDatasetControlWidget::on_Btn_Source_Dataset_Path_clicked()
{
    QString dir_name = QFileDialog::getExistingDirectory(0,"","");

    ui->textEdit_source_dataset_path->setText(dir_name + "/");
}

void MaskDatasetControlWidget::on_Btn_Target_Dataset_Path_clicked()
{
    QString dir_name = QFileDialog::getExistingDirectory(0,"","");

    ui->textEdit_target_dataset_path->setText(dir_name + "/");
}

void MaskDatasetControlWidget::on_Btn_Init_Param_clicked()
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

    init_param(source_dataset_path_string, target_dataset_path_string, target_dataset_size, workers_num);

    qDebug() << "Init Param finished.";
    ui->plainTextEdit_msg->appendPlainText("Init Param finished.");
}

void MaskDatasetControlWidget::on_Btn_Create_Work_Station_clicked()
{
    create_mask_work_station();
}

void MaskDatasetControlWidget::on_Btn_Recycle_Mask_clicked()
{
    create_dataset_struct();

    recycle_mask_dataset();
}

void MaskDatasetControlWidget::on_Btn_Create_Train_Dataset_clicked()
{
    get_absolute_file_path_list();

    create_train_dataset();
}

void MaskDatasetControlWidget::on_Btn_Create_Valid_Dataset_clicked()
{
    get_absolute_file_path_list();

    create_valid_dataset();
}

void MaskDatasetControlWidget::on_Btn_Recycle_Valid_clicked()
{
    recycle_valid_dataset();
}

void MaskDatasetControlWidget::create_dataset_struct(QString path_to_root)
{
    QDir dir_;
    if(!dir_.exists(target_path_))
    {
        dir_.mkpath(target_path_);
    }

    if(target_dataset_num_ < 0 || workers_num_ < 0)
    {
        return;
    }

    dir_.setPath(root_path_ + path_to_root);
    dir_.setFilter(QDir::Dirs | QDir::Files);
    dir_.setSorting(QDir::DirsFirst);

    QFileInfoList dir_list = dir_.entryInfoList();

    bool json_file_created = false;

    for(QFileInfo dir : dir_list)
    {
        if(dir.isDir() && dir.fileName() != "." && dir.fileName() != ".." && dir.fileName() != "work_station" && dir.fileName() != "train_dataset" && dir.fileName() != "valid_dataset")
        {
            QString current_path = root_path_ + path_to_root + dir.fileName();
            create_dataset_struct(path_to_root + dir.fileName() + "/");
        }
        else if(!json_file_created)
        {
            if(dir.fileName().contains(".jpg"))
            {
                QString current_path = root_path_ + path_to_root + "/Annotations";
                if(!dir_.exists(current_path))
                {
                    dir_.mkpath(current_path);
                }

                current_path = root_path_ + path_to_root + "/json";
                if(!dir_.exists(current_path))
                {
                    dir_.mkpath(current_path);
                }

                json_file_created = true;
            }
        }
    }
}

void MaskDatasetControlWidget::get_file_list(QString path_to_root)
{
    QDir dir_;

    dir_.setPath(root_path_ + path_to_root);
    dir_.setFilter(QDir::Dirs | QDir::Files);
    dir_.setSorting(QDir::DirsFirst);

    QFileInfoList file_list = dir_.entryInfoList();

    for(QFileInfo file : file_list)
    {
        if(file.isDir() && file.fileName() != "." && file.fileName() != ".." && file.fileName() != "work_station" && file.fileName() != "train_dataset" && file.fileName() != "valid_dataset")
        {
            get_file_list(path_to_root + file.fileName() + "/");
        }
        else if(file.fileName().contains(".jpg"))
        {
            source_file_path_.append(path_to_root + file.fileName());
            ++source_dataset_num_;

            if(!dir_.exists(root_path_ + path_to_root + "json/" + file.baseName() + ".json"))
            {
                unmarked_file_path_.append(path_to_root + file.fileName());
                ++unmarked_dataset_num_;
            }

            if(source_dataset_num_ % debug_skip_num_ == 0)
            {
                qDebug() << "found " << source_dataset_num_ << " images...";
                ui->plainTextEdit_msg->appendPlainText("found " + QString::number(source_dataset_num_) + " images...");
            }
        }
    }
}

void MaskDatasetControlWidget::get_absolute_file_path_list()
{
    if(!get_file_list_finished_)
    {
        source_dataset_num_ = 0;
        unmarked_dataset_num_ = 0;

        source_file_path_.clear();
        unmarked_file_path_.clear();

        get_file_list();

        get_file_list_finished_ = true;
    }

    if(unmarked_dataset_num_ == 0)
    {
        qDebug() << "All dataset are marked.";
        ui->plainTextEdit_msg->appendPlainText("All dataset are marked.");
        return;
    }

    qDebug() << "Dataset search finished. Now we have " << source_dataset_num_ << " images total; " << unmarked_dataset_num_ << " images still unmarked.";
    ui->plainTextEdit_msg->appendPlainText("Dataset search finished. Now we have " + QString::number(source_dataset_num_) + " images total; " + QString::number(unmarked_dataset_num_) + " images still unmarked.");

    if(target_dataset_num_ < 0)
    {
        return;
    }

    if(unmarked_dataset_num_ < target_dataset_num_)
    {
        qDebug() << "We don't have enough unmarked images, and they'll all be used.";
        ui->plainTextEdit_msg->appendPlainText("We don't have enough unmarked images, and they'll all be used.");
        use_all_unmarked_dataset_ = true;
    }
}

void MaskDatasetControlWidget::create_work_station()
{
    if(unmarked_dataset_num_ == 0 || workers_num_ < 0)
    {
        return;
    }

    QDir dir_;

    for(int i = 0; i < workers_num_; ++i)
    {
        if(!dir_.exists(work_path_ + QString::number(i)))
        {
            dir_.mkpath(work_path_ + QString::number(i));
        }
    }

    if(use_all_unmarked_dataset_)
    {
        dataset_num_for_each_worker_ = int(unmarked_dataset_num_ / workers_num_);
        res_dataset_num_ = unmarked_dataset_num_ % workers_num_;
    }
    else
    {
        dataset_num_for_each_worker_ = int(target_dataset_num_ / workers_num_);
        res_dataset_num_ = target_dataset_num_ % workers_num_;
    }

    qDebug() << "Work delivered finished. We have " << workers_num_ << " workers, and each worker has " << dataset_num_for_each_worker_ << " images to mark.";
    ui->plainTextEdit_msg->appendPlainText("Work delivered finished. We have " + QString::number(workers_num_) + " workers, and each worker has " + QString::number(dataset_num_for_each_worker_) + " images to mark.");

    if(res_dataset_num_ > 0)
    {
        qDebug() << "There are " << res_dataset_num_ << " images more, which will be marked randomly by workers.";
        ui->plainTextEdit_msg->appendPlainText("There are " + QString::number(res_dataset_num_) + " images more, which will be marked randomly by workers.");
    }
}

void MaskDatasetControlWidget::copy_unmarked_dataset()
{
    if(unmarked_dataset_num_ == 0 || workers_num_ < 0)
    {
        return;
    }

    QDir dir_;

    int skip_num = 1;
    if(dataset_num_for_each_worker_ > 0)
    {
        skip_num = int(unmarked_dataset_num_ / dataset_num_for_each_worker_ / workers_num_);
    }

    int current_unmarked_dataset_index = 0;

    int total_work_num = workers_num_ * dataset_num_for_each_worker_ + res_dataset_num_ < target_dataset_num_ ? workers_num_ * dataset_num_for_each_worker_ + res_dataset_num_ : target_dataset_num_;

    if(dataset_num_for_each_worker_ > 0)
    {
        for(int i = 0; i < workers_num_; ++i)
        {
            QString current_work_path = work_path_ + QString::number(i) + "/";

            QFile file(work_path_ + QString::number(i) + ".txt");

            if(file.open(QFile::WriteOnly))
            {
                file.resize(0);

                for(int j = 0; j < dataset_num_for_each_worker_; ++j)
                {
                    QString current_file_path = root_path_ + unmarked_file_path_[current_unmarked_dataset_index];

                    if(dir_.exists(current_work_path + QString::number(j) + ".jpg"))
                    {
                        dir_.remove(current_work_path + QString::number(j) + ".jpg");
                    }

                    if(!QFile::copy(current_file_path, current_work_path + QString::number(j) + ".jpg"))
                    {
                        qDebug() << "MaskDatasetControlWidget::copy_unmarked_dataset : copy failed.";
                        qDebug() << current_file_path;
                        qDebug() << current_work_path + QString::number(j) + ".jpg";
                        file.close();
                        return;
                    }

                    QString current_msg = QString::number(j) + "|chLi|" + current_file_path + "\n";

                    file.write(current_msg.toStdString().c_str());

                    current_unmarked_dataset_index += skip_num;

                    if((i * dataset_num_for_each_worker_ + j + 1) % debug_skip_num_ == 0)
                    {
                        qDebug() << "Copying files : " << i * dataset_num_for_each_worker_ + j + 1 << " / " << total_work_num << " ...";
                        ui->plainTextEdit_msg->appendPlainText("Copying files : " + QString::number(i * dataset_num_for_each_worker_ + j + 1) + " / " + QString::number(total_work_num) + " ...");
                    }
                }

                file.close();
            }
        }
    }

    current_unmarked_dataset_index -= skip_num - 1;

    for(int i = 0; i < res_dataset_num_; ++i)
    {
        QString current_work_path = work_path_ + QString::number(i) + "/";

        QFile file(work_path_ + QString::number(i) + ".txt");

        if(file.open(QFile::ReadWrite | QFile::Append))
        {
            int j = dataset_num_for_each_worker_;

            QString current_file_path = root_path_ + unmarked_file_path_[current_unmarked_dataset_index];

            if(!QFile::copy(current_file_path, current_work_path + QString::number(j) + ".jpg"))
            {
                qDebug() << "MaskDatasetControlWidget::copy_unmarked_dataset : copy failed.";
                qDebug() << current_file_path;
                qDebug() << current_work_path + QString::number(j) + ".jpg";
                file.close();
                return;
            }

            QString current_msg = QString::number(j) + "|chLi|" + current_file_path + "\n";

            file.write(current_msg.toStdString().c_str());

            ++current_unmarked_dataset_index;

            file.close();

            qDebug() << "Copying files : " << (workers_num_ - 1) * dataset_num_for_each_worker_ + i + j + 1 << " / " << total_work_num << "...";
            ui->plainTextEdit_msg->appendPlainText("Copying files : " + QString::number((workers_num_ - 1) * dataset_num_for_each_worker_ + i + j + 1) + " / " + QString::number(total_work_num) + "...");
        }
    }

    qDebug() << "Copy files finished.";
    ui->plainTextEdit_msg->appendPlainText("Copy files finished.");
}

void MaskDatasetControlWidget::init_param(QString root_path, QString target_path, int target_dataset_num, int workers_num)
{
    root_path_= root_path;
    target_path_ = target_path;
    target_dataset_num_ = target_dataset_num;
    workers_num_ = workers_num;

    source_dataset_num_ = 0;
    unmarked_dataset_num_ = 0;

    source_file_path_.clear();
    unmarked_file_path_.clear();

    dataset_num_for_each_worker_ = 0;
    res_dataset_num_ = 0;
    work_path_ = target_path_ + "work_station/";

    use_all_unmarked_dataset_ = false;

    if(root_path_copy_ != root_path_)
    {
        get_file_list_finished_ = false;
        root_path_copy_ = root_path_;
    }
}

void MaskDatasetControlWidget::create_mask_work_station()
{
    create_dataset_struct();

    get_absolute_file_path_list();

    create_work_station();

    copy_unmarked_dataset();
}

void MaskDatasetControlWidget::recycle_mask_dataset()
{
    QDir dir_;

    int dataset_recycled_num = 0;
    int dataset_unmarked_num = 0;
    int total_dataset_num = 0;

    for(int i = 0; i < workers_num_; ++i)
    {
        QFile file(work_path_ + QString::number(i) + ".txt");

        if(file.open(QFile::ReadOnly))
        {
            while(!file.atEnd())
            {
                file.readLine();
                ++total_dataset_num;
            }

            file.close();
        }
        else
        {
            qDebug() << "MaskDatasetControlWidget::recycle_mask_dataset : open recycle txt failed.";
            return;
        }
    }

    for(int i = 0; i < workers_num_; ++i)
    {
        QString current_work_path = work_path_ + QString::number(i) + "/";

        QStringList new_file_msg;

        QFile file(work_path_ + QString::number(i) + ".txt");

        if(file.open(QFile::ReadWrite))
        {
            while(!file.atEnd())
            {
                QByteArray line = file.readLine();

                QString str(line);

                QStringList str_list = str.split("|chLi|");

                QFileInfo file_info(str_list[1]);

                if(dir_.exists(current_work_path + "json/" + str_list[0] + ".json"))
                {
                    if(dir_.exists(file_info.path() + "/json/" + file_info.baseName() + ".json"))
                    {
                        dir_.remove(file_info.path() + "/json/" + file_info.baseName() + ".json");
                    }

                    if(!QFile::copy(current_work_path + "json/" + str_list[0] + ".json", file_info.path() + "/json/" + file_info.baseName() + ".json"))
                    {
                        qDebug() << "MaskDatasetControlWidget::recycle_mask_dataset : copy file failed.";
                        qDebug() << current_work_path + "json/" + str_list[0] + ".json";
                        qDebug() << file_info.path() + "/json/" + file_info.baseName() + ".json";
                        file.close();
                        return;
                    }

                    dir_.remove(current_work_path + "json/" + str_list[0] + ".json");

                    if(dir_.exists(file_info.path() + "/Annotations/" + file_info.baseName() + ".xml"))
                    {
                        dir_.remove(file_info.path() + "/Annotations/" + file_info.baseName() + ".xml");
                    }

                    QFile::copy(current_work_path + "Annotations/" + str_list[0] + ".xml", file_info.path() + "/Annotations/" + file_info.baseName() + ".xml");

                    dir_.remove(current_work_path + "Annotations/" + str_list[0] + ".xml");

                    dir_.remove(current_work_path + str_list[0] + ".jpg");

                    ++dataset_recycled_num;
                }
                else
                {
                    new_file_msg.append(str);

                    ++dataset_unmarked_num;
                }

                if((dataset_recycled_num + dataset_unmarked_num) % debug_skip_num_ == 0)
                {
                    qDebug() << "Recycling files : recycled/unmarked/total : " << dataset_recycled_num << " / " << dataset_unmarked_num << " / " << total_dataset_num << "...";
                    ui->plainTextEdit_msg->appendPlainText("Recycling files : recycled/unmarked/total : " + QString::number(dataset_recycled_num) + " / " + QString::number(dataset_unmarked_num) + " / " + QString::number(total_dataset_num) + "...");
                }
            }

            file.resize(0);

            for(QString str_ : new_file_msg)
            {
                file.write(str_.toStdString().c_str());
            }

            file.close();
        }
        else
        {
            qDebug() << "MaskDatasetControlWidget::recycle_mask_dataset : mask idx file open failed.";
            return;
        }
    }

    qDebug() << "Recycling finished : recycled/unmarked/total : " << dataset_recycled_num << " / " << dataset_unmarked_num << " / " << total_dataset_num;
    ui->plainTextEdit_msg->appendPlainText("Recycling finished : recycled/unmarked/total : " + QString::number(dataset_recycled_num) + " / " + QString::number(dataset_unmarked_num) + " / " + QString::number(total_dataset_num));
}

void MaskDatasetControlWidget::create_train_dataset()
{
    get_absolute_file_path_list();

    QDir dir_;
    if(!dir_.exists(root_path_ + "train_dataset"))
    {
        dir_.mkpath(root_path_ + "train_dataset");
    }

    int current_img_idx = 0;

    for(QString str : source_file_path_)
    {
        QFileInfo current_file_info(root_path_ + str);

        if(dir_.exists(current_file_info.path() + "/json/" + current_file_info.baseName() + ".json"))
        {
            if(dir_.exists(root_path_ + "train_dataset/" + QString::number(current_img_idx) + ".jpg"))
            {
                dir_.remove(root_path_ + "train_dataset/" + QString::number(current_img_idx) + ".jpg");
            }

            if(dir_.exists(root_path_ + "train_dataset/" + QString::number(current_img_idx) + ".json"))
            {
                dir_.remove(root_path_ + "train_dataset/" + QString::number(current_img_idx) + ".json");
            }

            if(!QFile::copy(current_file_info.filePath(), root_path_ + "train_dataset/" + QString::number(current_img_idx) + ".jpg"))
            {
                qDebug() << "MaskDatasetControlWidget::create_train_dataset : copy img failed.";
                qDebug() << current_file_info.filePath();
                qDebug() << root_path_ + "train_dataset/" + QString::number(current_img_idx) + ".jpg";
                return;
            }

            if(!QFile::copy(current_file_info.path() + "/json/" + current_file_info.baseName() + ".json", root_path_ + "train_dataset/" + QString::number(current_img_idx) + ".json"))
            {
                qDebug() << "MaskDatasetControlWidget::create_train_dataset : copy json failed.";
                qDebug() << current_file_info.path() + "/json/" + current_file_info.baseName() + ".json";
                qDebug() << root_path_ + "train_dataset/" + QString::number(current_img_idx) + ".json";
                return;
            }

            ++current_img_idx;

            if(current_img_idx % debug_skip_num_ == 0)
            {
                qDebug() << "Creating train dataset : " << current_img_idx << " / " << source_dataset_num_ - unmarked_dataset_num_ << " ...";
                ui->plainTextEdit_msg->appendPlainText("Creating train dataset : " + QString::number(current_img_idx) + " / " + QString::number(source_dataset_num_ - unmarked_dataset_num_) + " ...");
            }
        }
    }

    qDebug() << "Creating train dataset finished. Dataset size :" << current_img_idx;
    ui->plainTextEdit_msg->appendPlainText("Creating train dataset finished. Dataset size :" + QString::number(current_img_idx));
}

void MaskDatasetControlWidget::create_valid_dataset()
{
    QDir dir_;
    if(!dir_.exists(root_path_ + "valid_dataset"))
    {
        dir_.mkpath(root_path_ + "valid_dataset");
    }
    if(!dir_.exists(root_path_ + "valid_dataset/json"))
    {
        dir_.mkpath(root_path_ + "valid_dataset/json");
    }

    int current_img_idx = 0;
    int current_process_num = 0;

    QFile file(root_path_ + "valid_dataset/0.txt");

    if(file.open(QFile::WriteOnly))
    {
        file.resize(0);

        for(QString str : source_file_path_)
        {
            ++current_process_num;

            QString current_file_path = root_path_ + str;

            QFileInfo current_file_info(current_file_path);

            if(dir_.exists(current_file_info.path() + "/json/" + current_file_info.baseName() + ".json"))
            {
                QFile json_file(current_file_info.path() + "/json/" + current_file_info.baseName() + ".json");

                if(json_file.open(QFile::ReadOnly))
                {
                    QByteArray array = json_file.readAll();

                    QJsonDocument doc_test = QJsonDocument::fromJson(array);
                    QJsonObject obj_test = doc_test.object();

                    if(obj_test.value("Area").toObject().value("labels").toArray().size() > 0)
                    {
                        if(dir_.exists(root_path_ + "valid_dataset/" + QString::number(current_img_idx) + ".jpg"))
                        {
                            dir_.remove(root_path_ + "valid_dataset/" + QString::number(current_img_idx) + ".jpg");
                        }

                        if(dir_.exists(root_path_ + "valid_dataset/json/" + QString::number(current_img_idx) + ".json"))
                        {
                            dir_.remove(root_path_ + "valid_dataset/json/" + QString::number(current_img_idx) + ".json");
                        }

                        if(!QFile::copy(current_file_info.filePath(), root_path_ + "valid_dataset/" + QString::number(current_img_idx) + ".jpg"))
                        {
                            qDebug() << "MaskDatasetController::create_valid_dataset : copy img failed.";
                            qDebug() << current_file_info.filePath();
                            qDebug() << root_path_ + "valid_dataset/" + QString::number(current_img_idx) + ".jpg";
                            return;
                        }

                        if(!QFile::copy(current_file_info.path() + "/json/" + current_file_info.baseName() + ".json", root_path_ + "valid_dataset/json/" + QString::number(current_img_idx) + ".json"))
                        {
                            qDebug() << "MaskDatasetController::create_valid_dataset : copy json failed.";
                            qDebug() << current_file_info.path() + "/json/" + current_file_info.baseName() + ".json";
                            qDebug() << root_path_ + "valid_dataset/json/" + QString::number(current_img_idx) + ".json";
                            return;
                        }

                        QString current_msg = QString::number(current_img_idx) + "|chLi|" + current_file_path + "\n";

                        file.write(current_msg.toStdString().c_str());

                        ++current_img_idx;

                        if(current_img_idx % debug_skip_num_ == 0)
                        {
                            qDebug() << "Creating valid dataset : " << current_img_idx << " / " << source_dataset_num_ - unmarked_dataset_num_ << " ...";
                            ui->plainTextEdit_msg->appendPlainText("Creating valid dataset : " + QString::number(current_img_idx) + " / " + QString::number(source_dataset_num_ - unmarked_dataset_num_) + " ...");
                        }
                    }
                }
                else
                {
                    qDebug() << "MaskDatasetController::create_valid_dataset : json_file read failed";
                    return;
                }
            }
        }

        file.close();
    }

    qDebug() << "Creating valid dataset finished. Dataset size :" << current_img_idx;
    ui->plainTextEdit_msg->appendPlainText("Creating valid dataset finished. Dataset size :" + QString::number(current_img_idx));
}

void MaskDatasetControlWidget::recycle_valid_dataset()
{
    QDir dir_;

    int dataset_recycled_num = 0;
    int dataset_unmarked_num = 0;
    int total_dataset_num = 0;

    QFile file_list(root_path_ + "valid_dataset/0.txt");

    if(file_list.open(QFile::ReadOnly))
    {
        while(!file_list.atEnd())
        {
            file_list.readLine();
            ++total_dataset_num;
        }

        file_list.close();
    }
    else
    {
        qDebug() << "MaskDatasetControlWidget::recycle_valid_dataset : open recycle txt failed.";
        return;
    }

    QString current_work_path = root_path_ + "valid_dataset/";

    QStringList new_file_msg;

    QFile file(root_path_ + "valid_dataset/0.txt");

    if(file.open(QFile::ReadWrite))
    {
        while(!file.atEnd())
        {
            QByteArray line = file.readLine();

            QString str(line);

            QStringList str_list = str.split("|chLi|");

            QFileInfo file_info(str_list[1]);

            if(dir_.exists(current_work_path + "json/" + str_list[0] + ".json"))
            {
                if(dir_.exists(file_info.path() + "/json/" + file_info.baseName() + ".json"))
                {
                    dir_.remove(file_info.path() + "/json/" + file_info.baseName() + ".json");
                }

                if(!QFile::copy(current_work_path + "json/" + str_list[0] + ".json", file_info.path() + "/json/" + file_info.baseName() + ".json"))
                {
                    qDebug() << "MaskDatasetControlWidget::recycle_valid_dataset : copy file failed.";
                    qDebug() << current_work_path + "json/" + str_list[0] + ".json";
                    qDebug() << file_info.path() + "/json/" + file_info.baseName() + ".json";
                    file.close();
                    return;
                }

                dir_.remove(current_work_path + "json/" + str_list[0] + ".json");

                if(dir_.exists(file_info.path() + "/Annotations/" + file_info.baseName() + ".xml"))
                {
                    dir_.remove(file_info.path() + "/Annotations/" + file_info.baseName() + ".xml");
                }

                QFile::copy(current_work_path + "Annotations/" + str_list[0] + ".xml", file_info.path() + "/Annotations/" + file_info.baseName() + ".xml");

                dir_.remove(current_work_path + "Annotations/" + str_list[0] + ".xml");

                dir_.remove(current_work_path + str_list[0] + ".jpg");

                ++dataset_recycled_num;
            }
            else
            {
                new_file_msg.append(str);

                ++dataset_unmarked_num;
            }

            if((dataset_recycled_num + dataset_unmarked_num) % debug_skip_num_ == 0)
            {
                qDebug() << "Recycling files : recycled/unmarked/total : " << dataset_recycled_num << " / " << dataset_unmarked_num << " / " << total_dataset_num << "...";
                ui->plainTextEdit_msg->appendPlainText("Recycling files : recycled/unmarked/total : " + QString::number(dataset_recycled_num) + " / " + QString::number(dataset_unmarked_num) + " / " + QString::number(total_dataset_num) + "...");
            }
        }

        file.resize(0);

        for(QString str_ : new_file_msg)
        {
            file.write(str_.toStdString().c_str());
        }

        file.close();
    }
    else
    {
        qDebug() << "MaskDatasetControlWidget::recycle_valid_dataset : mask idx file open failed.";
        return;
    }

    qDebug() << "Recycling finished : recycled/unmarked/total : " << dataset_recycled_num << " / " << dataset_unmarked_num << " / " << total_dataset_num;
    ui->plainTextEdit_msg->appendPlainText("Recycling finished : recycled/unmarked/total : " + QString::number(dataset_recycled_num) + " / " + QString::number(dataset_unmarked_num) + " / " + QString::number(total_dataset_num));
}
