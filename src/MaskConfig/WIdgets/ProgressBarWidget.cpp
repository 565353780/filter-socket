#include "ProgressBarWidget.h"
#include "ui_progressbarwidget.h"

ProgressBarWidget::ProgressBarWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProgressBarWidget)
{
    ui->setupUi(this);

    max_value_ = 1;
}

ProgressBarWidget::~ProgressBarWidget()
{
    delete ui;
}

void ProgressBarWidget::setMaxValue(int max_value)
{
    if(max_value > 0)
    {
        this->ui->progressBar->setMaximum(max_value);

        this->ui->progressBar->setValue(1);

        max_value_ = max_value;

        this->ui->lineEdit_CurrentIndex->setText(QString::number(1));

        this->ui->label_Msg->setText(QString::number(1) + " / " + QString::number(max_value_));
    }
}

void ProgressBarWidget::setValue(int value)
{
    int value_show = value + 1;

    if(value_show > 0 && value_show <= max_value_)
    {
        this->ui->progressBar->setValue(value_show);
    }

    this->ui->lineEdit_CurrentIndex->setText(QString::number(value_show));

    this->ui->label_Msg->setText(QString::number(value_show) + " / " + QString::number(max_value_));

    this->update();
}

void ProgressBarWidget::on_Btn_ChangeTo_clicked()
{
    QString target_idx_string = this->ui->lineEdit_CurrentIndex->text();

    if(target_idx_string != "")
    {
        int target_idx = target_idx_string.toInt();

        setValue(target_idx);

        emit progressChangeTo(target_idx);
    }
    else
    {
        qDebug() << "ProgressBarWidget::on_Btn_ChangeTo_clicked : invalid target_idx.";
    }
}
