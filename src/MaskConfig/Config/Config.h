#ifndef MaskConfig_CONFIG_H
#define MaskConfig_CONFIG_H
#include <memory>
#include <QList>
#include <QMap>
#include <QString>
#include <QDir>
class QJsonObject;
namespace MaskConfig{
class Setting;
/**
 * @brief 保存含"type"的jsonObject的类
 */
class Config
{
public:
    Config();
    virtual ~Config() {}

    void init(const QString& filename);
    void load(const QString& filename);
    void json2xml(const QString& filename, bool need_to_change_image_size=false, bool use_my_labels=false, int image_width=512, int image_height=512);
    void save(const QString& filename);

    /**
     * @brief 添加Setting
     */
    void add(const QString& name, std::shared_ptr<Setting> setting);
    void clear();
    QList<QString> keys() const;
    Setting* getSetting(const QString& key);

    QString getName(const QString& key);
protected:
    /**
     * @brief 从JsonObject中读取信息并添加新的Setting
     * @param name
     * @param object
     */
    void addFromJsonObject(const QString& name, const QJsonObject& object);
private:
   QMap<QString, std::shared_ptr<Setting> > settings_;
};
}
#endif // CONFIG_H
