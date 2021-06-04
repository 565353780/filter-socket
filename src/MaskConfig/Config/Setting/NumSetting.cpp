#include "NumSetting.h"
#include "SettingWords.h"

namespace MaskConfig{

NumSetting::NumSetting():min_value_(-999.0),max_value_(999.0),default_value_(0.0)
{
    has_setter_ = false;
    type_ = SettingTypes::Float;

}

void NumSetting::loadFromJsonObject(const QJsonObject &object)
{
    Setting::loadFromJsonObject(object);


    if(object_.contains(SettingWords::NumValue))
    {
        default_value_ = value_ = object_[SettingWords::NumValue].toDouble();
    }
    if(object_.contains(SettingWords::MaxValue))
    {
        max_value_ = object_[SettingWords::MaxValue].toDouble();
    }
    if(object_.contains(SettingWords::MinValue))
    {
        min_value_ = object_[SettingWords::MinValue].toDouble();
    }

    this->update();
}

void NumSetting::saveAsJsonObject()
{

    Setting::saveAsJsonObject();
    object_[SettingWords::MaxValue] = max_value_;
    object_[SettingWords::MinValue] = min_value_;
    object_[SettingWords::NumValue] = value_;
}

float NumSetting::min_value() const
{
    return min_value_;
}

float NumSetting::max_value() const
{
    return max_value_;
}

float NumSetting::default_value() const
{
    return default_value_;
}

float NumSetting::value() const
{
    return value_;
}

void NumSetting::setValue(float v)
{
    value_ = v;
}

void NumSetting::setDefault()
{
    value_ = default_value_;
}
}
