#ifndef POLYGONLISTSETTING_H
#define POLYGONLISTSETTING_H
#include "Setting.h"
#include <QPolygon>
#include <QColor>
#include <memory>
namespace MaskConfig{


class PolygonListSetting;
class LabelDict;
/**
 * @brief 包含"type"为"polygons"的JsonObject的类
 */
class PolygonListSetting : public Setting
{
public:
    PolygonListSetting();
    virtual ~PolygonListSetting() {}
public:
    virtual void initFromJsonObject(const QJsonObject &object);
    virtual void loadFromJsonObject(const QJsonObject& object);
    virtual void saveAsJsonObject();
    /**
     * @brief 是否可以保存
     * @return
     */
    virtual bool isReadyToSave();

public:
    virtual void add(const QPolygonF& poly);
    virtual void remove(int index);
    virtual void clear();
    virtual void pop_back();


public:
    int poly_size() const {return polygons_.size();}
    /**
     * @brief 返回选中Poly的颜色
     * @return
     */
    QColor selectedColor() const;
    QColor color() const;

    QStringList targetKeys() const;

public:
    QColor color_;
    QColor selectedColor_;
    QList< std::shared_ptr<LabelDict> > label_dict_list_;
    QList< QStringList > labels_;
    QList< QMap<QString, int> > bindings_;
    QList<QPolygonF> polygons_;
    QSizeF v_size_;
    bool  has_labels_;
    QStringList targetKeys_;


};
}
#endif // POLYGONLISTSETTING_H
