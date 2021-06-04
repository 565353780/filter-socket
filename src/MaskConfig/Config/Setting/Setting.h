#ifndef MaskConfigSETTING_H
#define MaskConfigSETTING_H
#include <QJsonObject>
#include <QString>
#include "Util/LogTool.h"
#include <QObject>
namespace MaskConfig{

namespace SettingTypes {
    const QString Int = "int";
    const QString Text = "text";
    const QString Float = "float";
    const QString String = "string";
    const QString Polygon = "polygon";
    const QString Polygons = "polygons";
    const QString BBox = "bbox";
    const QString Select = "select";
}
/**
 * @brief 包含JsonObject及基本信息的基础类
 */
class Setting : public QObject
{
    Q_OBJECT
public:
    Setting();
    virtual ~Setting(){}

    bool has_setter() const;
    virtual void clear(){}

    QString type() const {return type_;}
    QString category() const {return category_;}
    QString name() const {return ui_name_;}

    /**
     * @brief 返回name对应的QJsonValue
     */
    QJsonValue getValue(const QString& name) const;

    const QJsonObject& object() const {return object_;}

signals:
    void signalUpdate(int state = 1);
    void signalInfoUpdate();
public:
    void updateInfo();
    void update(int state = 1);
public:
    /**
     * @brief 读取JsonObject信息并初始化
     * @param object
     */
    virtual void initFromJsonObject(const QJsonObject &object);
    /**
     * @brief 加载JsonObject信息
     * @param object
     */
    virtual void loadFromJsonObject(const QJsonObject& object);
    /**
     * @brief 保存type信息至JsonObject
     */
    virtual void saveAsJsonObject();
    virtual bool isReadyToSave() {return true;}




protected:
    QJsonObject object_;

protected:
    QString type_;
    QString category_;
    QString ui_name_;
protected:
    bool has_setter_;

};
}
#endif // SETTING_H
