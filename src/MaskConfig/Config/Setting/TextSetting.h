#ifndef TEXTSETTING_H
#define TEXTSETTING_H
#include "Setting.h"
namespace MaskConfig{
/**
 * @brief 包含"type"为"Value"的JsonObject的类
 */
class TextSetting : public Setting
{
public:
    TextSetting();
    virtual ~TextSetting() {}
    QString text() const;
    void setText(const QString &text);
public:
    void loadFromJsonObject(const QJsonObject& object);
    void saveAsJsonObject();
private:
    QString text_;
};

}

#endif // TEXTSETTING_H
