#include "SettingRowWidget.h"
#include "Config/Setting/NumSetting.h"
#include "Config/Setting/SelectSetting.h"
#include "Config/Setting/TextSetting.h"
#include <QValidator>
#include <QIntValidator>
#include <QDoubleValidator>
#include <QLabel>
#include <QButtonGroup>
#include <QPushButton>
#include <QCheckBox>
namespace MaskConfig {


NumSettingWidget::NumSettingWidget(NumSetting *setting, QWidget *parent):QLineEdit(parent),setting_(setting)
{
    if(setting->type() == SettingTypes::Float)
    {
        this->setValidator(new QDoubleValidator(setting->min_value(),setting->max_value(),2,this));
        this->setText(QString::number(setting->value()));

    }
    else {
        this->setValidator(new QIntValidator((int)setting->min_value(),(int)setting->max_value(),this));
        this->setText(QString::number((int)setting->value()));
    }

    connect(setting_,SIGNAL(signalUpdate()),this,SLOT(updateState()));
    connect(this,SIGNAL(textChanged(QString)),this,SLOT(slotTextChanged(QString)));

    this->resize(150,30);
}

void NumSettingWidget::slotTextChanged(QString text)
{
    bool ok;
    double val = text.toDouble(&ok);
    if(ok)
    {
        setting_->setValue((float)val);
        LogTool::debug("NumSettingWidget::slotTextChanged Successed", text);
    }
    else
    {
        LogTool::debug("NumSettingWidget::slotTextChanged Failed", text);
    }
}

void NumSettingWidget::updateState()
{
    if(setting_->type() == SettingTypes::Float)
    {
        this->setText(QString::number(setting_->value()));

    }
    else {
        this->setText(QString::number((int)setting_->value()));
    }
}



SingleSelectSettingWidget::SingleSelectSettingWidget(SelectSetting *setting, QWidget *parent):QComboBox(parent),setting_(setting)
{
    foreach (QString key, setting_->option_keys_) {
        this->addItem(setting_->options_[key],QVariant(key));
    }
    connect(setting,SIGNAL(signalUpdate(int)),this,SLOT(updateState(int)));

    connect(this,SIGNAL(currentIndexChanged(int)),this,SLOT(slotChanged(int)));

    this->resize(150,30);
}

void SingleSelectSettingWidget::updateState(int state)
{

    if(!state)
    {
        this->setCurrentIndex(0);
        return;
    }
    if(setting_->valuesMap_.size() > 0)
    {
        this->setCurrentText(setting_->options_[setting_->valuesMap_.firstKey()]);
    }
    else
    {
        QString key = this->currentData().toString();
        setting_->selectKey(key,1);
    }
}

void SingleSelectSettingWidget::slotChanged(int index)
{
    QString key = this->currentData().toString();
    setting_->selectKey(key,1);
}

SettingRowWidget::SettingRowWidget(Setting *setting, QWidget *parent):QWidget(parent),setting_(setting),val_widget_(NULL)
{
    this->init();
}

void SettingRowWidget::init()
{

    if(setting_->type() == SettingTypes::Float || setting_->type()== SettingTypes::Int)
    {
        NumSetting* num_setting = dynamic_cast<NumSetting * >(setting_);
        if(num_setting)
        {
            val_widget_ = new NumSettingWidget(num_setting,this);
        }
    }
    else if(setting_->type() == SettingTypes::Text)
    {
        TextSetting *text_setting = dynamic_cast<TextSetting * >(setting_);
        if(text_setting)
        {
            val_widget_ = new TextSettingWidget(text_setting,this);
        }
    }
    if(setting_->type() == SettingTypes::Select)
    {

        SelectSetting* select_setting = dynamic_cast<SelectSetting * >(setting_);
        if(select_setting)
        {
            if(select_setting->checkbox())
            {
                val_widget_ = new MultiSelectSettingWidget(select_setting,this);
            }
            else
            {
                val_widget_ = new SingleSelectSettingWidget(select_setting,this);
            }
        }
    }
    label_ = new QLabel(this);
    label_->setText(setting_->name());
    label_->move(15,8);
    if(val_widget_)
    {
        val_widget_->move(100,0);
        this->resize(250,val_widget_->height());
    }
    else
    {
        this->resize(250,30);
    }
}

MultiSelectSettingWidget::MultiSelectSettingWidget(SelectSetting *setting, QWidget *parent) : QWidget(parent),setting_(setting)
{

    int y = 0;
    for(int i=0; i <setting->option_keys_.size(); i++)
    {
        QString key =  setting->option_keys_[i];
        QCheckBox* check = new QCheckBox(setting->options_[key],this);
        connect(check,SIGNAL(clicked(bool)),this,SLOT(slotChangeState()));
        checkbox_map_[key] = check;
        check->move(i, i * 25 + 5);
        y = check->y() + 25;
    }
    this->resize(250,y);
    connect(setting,SIGNAL(signalUpdate()),this,SLOT(updateState()));
}

void MultiSelectSettingWidget::updateState()
{
    foreach (const QString& key, setting_->option_keys_) {
        checkbox_map_[key]->setChecked(false);
        if(setting_->valuesMap_.contains(key))
        {
            checkbox_map_[key]->setChecked((setting_->valuesMap_[key]==1));
        }
    }
}

void MultiSelectSettingWidget::slotChangeState()
{
    setting_->valuesMap_.clear();
    foreach (const QString& key, setting_->option_keys_) {
        if(checkbox_map_[key]->isChecked())
        {
            setting_->valuesMap_[key] = 1;
        }
    }
}

TextSettingWidget::TextSettingWidget(TextSetting *setting, QWidget *parent):QLineEdit(parent),setting_(setting)
{
    this->setText(setting_->text());
    connect(setting_,SIGNAL(signalUpdate()),this,SLOT(updateState()));
    connect(this,SIGNAL(textChanged(QString)),this,SLOT(slotTextChanged(QString)));

    this->resize(150,30);
}

void TextSettingWidget::slotTextChanged(QString text)
{
    setting_->setText(text);
}

void TextSettingWidget::updateState()
{
    this->setText(setting_->text());
}



}
