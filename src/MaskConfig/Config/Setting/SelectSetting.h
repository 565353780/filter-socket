#ifndef SELECTSETTING_H
#define SELECTSETTING_H
#include "Setting.h"
namespace MaskConfig {
/**
 * @brief 包含"type"为"select"的JsonObject的类
 */
class SelectSetting : public Setting
{
public:
    SelectSetting();
public:
    virtual void loadFromJsonObject(const QJsonObject& object);
    virtual void saveAsJsonObject();


    void selectKey(const QString& key, int state = 1);

public:
    bool checkbox() const;

public:
    bool checkbox_;
    QStringList option_keys_;
    QMap<QString, QString> options_;



    QMap<QString, int> valuesMap_;

};
}
#endif // SELECTSETTING_H
