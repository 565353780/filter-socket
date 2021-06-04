#include "Setting.h"
#include "SettingWords.h"
namespace MaskConfig{
Setting::Setting():has_setter_(false)
{
}

void Setting::loadFromJsonObject(const QJsonObject &object)
{
    if(object.contains(SettingWords::Type))
    {
        type_ = object[SettingWords::Type].toString();
    }
    if(object.contains(SettingWords::Category))
    {
        category_ = object[SettingWords::Category].toString();
    }
    if(object.contains(SettingWords::UI_Name))
    {
        ui_name_ = object[SettingWords::UI_Name].toString();
    }
    object_ = object;
}

void Setting::saveAsJsonObject()
{
    object_[SettingWords::Type] = QJsonValue(type_);
//    object_[SettingWords::Category] = QJsonValue(category_);
//    object_[SettingWords::UI_Name] = QJsonValue(ui_name_);

}

bool Setting::has_setter() const
{
    return has_setter_;
}

QJsonValue Setting::getValue(const QString &name) const
{
     if(object_.contains(name))
     {
         return object_[name];
     }
     else
     {
         return QJsonValue();
     }
}

void Setting::updateInfo()
{
    emit signalInfoUpdate();
}

void Setting::update(int state)
{
    emit signalUpdate(state);
}

void Setting::initFromJsonObject(const QJsonObject &object)
{
    this->loadFromJsonObject(object);
}
}
