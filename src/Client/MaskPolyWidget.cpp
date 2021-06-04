#include "MaskPolyWidget.h"
#include "ui_maskpolywidget.h"

MaskPolyWidget::MaskPolyWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MaskPolyWidget)
{
    ui->setupUi(this);

    mask_draw_widget = new MaskDrawWidget(this);

    connect(mask_draw_widget, SIGNAL(poly_changed()), this, SLOT(on_MaskDrawWidget_Poly_Changed()));

    ui->HLayout_mask_poly_img->addWidget(mask_draw_widget);

    mask_draw_widget->show();

    empty_group = new QGroupBox();

    ui->textEdit_LabelInput->setText("label example");

    on_Btn_EditLabel_clicked();
}

MaskPolyWidget::~MaskPolyWidget()
{
    delete ui;
}

void MaskPolyWidget::on_Btn_OpenDir_clicked()
{
    QString srcDirPath = QFileDialog::getExistingDirectory(this, "choose src Directory", "/");

    if (srcDirPath.isEmpty())
    {
        return;
    }
    else
    {
        qDebug() << "srcDirPath=" << srcDirPath;
        srcDirPath += "/";
    }

    mask_draw_widget->get_File_List(srcDirPath);
}

void MaskPolyWidget::on_Btn_PreImg_clicked()
{
    mask_draw_widget->show_Previous_Img();
}

void MaskPolyWidget::on_Btn_NextImg_clicked()
{
    mask_draw_widget->show_Next_Img();
}

void MaskPolyWidget::on_Btn_EditLabel_clicked()
{
    QString labels_string = ui->textEdit_LabelInput->toPlainText();

    if(labels_string == labels_string_copy)
    {
        qDebug() << "label input doesn't change !" << endl;

        return;
    }

    labels_string_copy.clear();

    if(labels_string == "")
    {
        qDebug() << "label input is empty !" << endl;

        return;
    }

    label_list.clear();

    label_list = labels_string.split(",");

    ui->cBox_Label->clear();

    for(int i = 0; i < label_list.size(); ++i)
    {
        QString currentlabel = label_list.at(i).simplified();

        if(currentlabel != "")
        {
            if(labels_string_copy != "")
            {
                labels_string_copy += ",";
            }

            labels_string_copy += currentlabel;

            ui->cBox_Label->addItem(label_list.at(i).simplified());
        }
    }

    ui->textEdit_LabelInput->setText(labels_string_copy);

    ui->cBox_Label->setCurrentIndex(0);
}

void MaskPolyWidget::on_MaskDrawWidget_Poly_Changed()
{
    if(mask_draw_widget->mask_episode_list.size() < label_index_list.size())
    {
        label_index_list.pop_back();
    }
    else if(mask_draw_widget->mask_episode_list.size() > label_index_list.size())
    {
        label_index_list.append(ui->cBox_Label->currentIndex());
    }

    if(btn_list.size() > 0)
    {
        for(int i = 0; i < btn_list.size(); ++i)
        {
            ui->VLayout_Label_Btns->removeWidget(btn_list[i]);
            delete(btn_list[i]);
        }

        btn_list.clear();

        ui->VLayout_Label_Btns->removeWidget(empty_group);
    }

    for(int i = 0; i < mask_draw_widget->mask_episode_list.size(); ++i)
    {
        QPushButton* btn_label = new QPushButton(tr((QString::number(i) + ":" + label_list.at(label_index_list[i])).toStdString().c_str()));
        btn_list.append(btn_label);
        ui->VLayout_Label_Btns->addWidget(btn_label, 1);
    }

    ui->VLayout_Label_Btns->addWidget(empty_group, 100);
}

void MaskPolyWidget::on_cBox_Label_currentIndexChanged(int index)
{
    if(label_index_list.size() > 0)
    {
        label_index_list[mask_draw_widget->current_activate_poly] = index;

        on_MaskDrawWidget_Poly_Changed();
    }
}

void MaskPolyWidget::on_Btn_Label_Pressed()
{
    for(int i = 0; i < btn_list.size(); ++i)
    {
//        if(btn_list[i]->is())
//        {
//            mask_draw_widget->current_activate_poly = index;
//            mask_draw_widget->update();

//            break;
//        }
    }
}
