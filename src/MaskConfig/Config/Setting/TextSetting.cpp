#include "TextSetting.h"
#include <QJsonObject>
#include "SettingWords.h"
namespace MaskConfig{

TextSetting::TextSetting()
{

}

QString TextSetting::text() const
{
    return text_;
}

void TextSetting::setText(const QString &text)
{
    text_ = text;
}

void TextSetting::loadFromJsonObject(const QJsonObject &object)
{
    Setting::loadFromJsonObject(object);


    if(object_.contains(SettingWords::Value))
    {
        text_ = object_[SettingWords::Value].toString();
    }


    this->update();
}

void TextSetting::saveAsJsonObject()
{
    Setting::saveAsJsonObject();
    object_[SettingWords::Value] = text_;
}

}
