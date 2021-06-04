#include "SelectSetting.h"
#include "SettingWords.h"
#include <QJsonArray>
#include <QJsonObject>
namespace MaskConfig {

SelectSetting::SelectSetting()
{

}

void SelectSetting::loadFromJsonObject(const QJsonObject &object)
{
    Setting::loadFromJsonObject(object);
    if(object.contains(SettingWords::Options))
    {
        options_.clear();
        option_keys_.clear();
        QJsonArray array = object[SettingWords::Options].toArray();
        for(int i=0; i < array.size(); i++)
        {
            QJsonArray key_val = array[i].toArray();
            if(key_val.size() < 2) continue;
            QString key = key_val[0].toString();
            QString value = key_val[1].toString();
            options_[key] = value;
            option_keys_.push_back(key);
        }
    }
    if(object.contains(SettingWords::CheckBox))
    {
        checkbox_ = false;
        QString str = object[SettingWords::CheckBox].toString();
        checkbox_ = (str.toUpper() != "FALSE");
    }
    if(object.contains(SettingWords::Values))
    {
        valuesMap_.clear();
        QJsonArray array = object[SettingWords::Values].toArray();
        for(int i=0; i < array.size(); i++)
        {
            QString key = array[i].toString();
            if(options_.contains(key))
            {
                valuesMap_[key] = 1;
            }
        }

    }

    object_ = QJsonObject();
    this->update();

}

void SelectSetting::saveAsJsonObject()
{
    object_ = QJsonObject();
    Setting::saveAsJsonObject();

    QJsonArray val_array;
    foreach (const QString& key, option_keys_)
    {
        if(valuesMap_.contains(key))
        {
            if(valuesMap_[key])
            {
                val_array.push_back(QJsonValue(key));
            }
        }
    }
    object_[SettingWords::Values] = val_array;


}

void SelectSetting::selectKey(const QString &key, int state)
{
    if(options_.contains(key))
    {
        if(!checkbox())
        {
            valuesMap_.clear();
        }
        valuesMap_[key] = state;
    }
}

bool SelectSetting::checkbox() const
{
    return checkbox_;
}

}
