#include "LabelDictWidget.h"
#include <QComboBox>
#include "Config/Setting/LabelDict.h"
#include <QLabel>
#include <QKeyEvent>
#include <QDebug>
namespace MaskConfig {
LabelDictWidget::LabelDictWidget(LabelDict *dict, QWidget *parent):QWidget(parent),dict_(dict)
{
    this->init();
}

void LabelDictWidget::init()
{
    label_ = new QLabel(dict_->ui_name(),this);
    combobox_ = new QComboBox(this);
    foreach (const QString& key, dict_->option_keys_)
    {
        combobox_->addItem(dict_->options_[key], key);
    }
    key_ = dict_->key_;

    combobox_->setCurrentText(dict_->options_[key_]);
    combobox_->clearFocus();

    label_->setGeometry(3,-10,60,40);
    combobox_->setGeometry(100,-10,150,40);
    combobox_->setFont(QFont("Microsoft Yahei",8));
    connect(combobox_,SIGNAL(currentIndexChanged(int)),this,SLOT(slotChanged(int)));

    key_input_num_ = 0;
}

void LabelDictWidget::keyInput(int num)
{
    if(num == -1)
    {
        key_input_num_ = 0;
    }
    else
    {
        if(key_input_num_ * 10 + num < dict_->options_.size())
        {
            key_input_num_ = key_input_num_ * 10 + num;
        }
        else
        {
            key_input_num_ = num;
        }
    }

    combobox_->setCurrentIndex(key_input_num_);
}

void LabelDictWidget::resizeEvent(QResizeEvent *event)
{

}

void LabelDictWidget::slotChanged(int index)
{
    key_ = combobox_->currentData().toString();
    dict_->key_ = key_;
    emit signalChanged();
    //qDebug()<<"found combobox change"<<key_;
}

void LabelDictWidget::slotUpdateValue()
{
    key_ = dict_->key_;
    combobox_->setCurrentText(dict_->options_[dict_->key_]);
}
}
