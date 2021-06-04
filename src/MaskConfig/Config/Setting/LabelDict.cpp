#include "LabelDict.h"
#include "SettingWords.h"
#include <QDebug>
#include <QJsonArray>
namespace MaskConfig {
LabelDict::LabelDict()
{

}

void LabelDict::loadFromJsonObject(const QJsonObject &object)
{
    if(object.contains(SettingWords::Name))
    {
        name_ = object[SettingWords::Name].toString();
    }
    if(object.contains(SettingWords::UI_Name))
    {
        ui_name_ = object[SettingWords::UI_Name].toString();
    }
    if(object.contains(SettingWords::Options))
    {
//        QJsonObject options_object = object[SettingWords::Options].toObject();
//        QStringList keys = options_object.keys();
//        options_.clear();
//        foreach (QString key, keys) {
//            qDebug()<<key;
//            options_[key] = options_object[key].toString();
//        }
        options_.clear();
        option_keys_.clear();
        QJsonArray options_array = object[SettingWords::Options].toArray();
        for(int i=0; i < options_array.size(); i++)
        {
            QJsonArray kval_array = options_array[i].toArray();
            if(kval_array.size() < 2) continue;
            QString key = kval_array[0].toString();
            QString value = kval_array[1].toString();
            options_[key] = value;
            option_keys_.push_back(key);

        }
    }
    if(object.contains(SettingWords::Default))
    {
        key_ = object[SettingWords::Default].toString();
        if(!options_.contains(key_))
        {
            key_ = options_.firstKey();
        }
    }

}

QString LabelDict::option(const QString &key) const
{
    if(options_.contains(key))
    {
        return options_[key];
    }
    else
    {
        return "";
    }
}

QString LabelDict::ui_name() const
{
    if(ui_name_.isEmpty())
    {
        return name_;
    }
    return ui_name_;
}
}
