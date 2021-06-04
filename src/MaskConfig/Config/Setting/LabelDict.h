#ifndef LABELDICT_H
#define LABELDICT_H
#include <QJsonObject>
#include <QString>
#include <QMap>
#include <QStringList>
namespace MaskConfig {
/**
 * @brief 保存"labelDict"jsonObject的信息类
 */
class LabelDict
{
public:
    LabelDict();
    /**
     * @brief 从JsonObject中读取信息
     * @param JsonObject
     */
    void loadFromJsonObject(const QJsonObject& object);

    /**
     * @brief 返回option中key的对应字符串
     * @param key
     * @return 对应字符串
     */
    QString option(const QString& key) const;

    QString ui_name() const;
public:
    QString name_;
    QString ui_name_;
    QString key_;


    QStringList option_keys_;
    QMap<QString,QString> options_;

};
}
#endif // LABELDICT_H
