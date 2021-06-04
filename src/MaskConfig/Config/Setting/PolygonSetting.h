#ifndef POLYGONSETTING_H
#define POLYGONSETTING_H
#include "Setting.h"
#include <QPolygon>
#include <QColor>
namespace MaskConfig{
/**
 * @brief 包含"type"为"polygon"的JsonObject的类
 */
class PolygonSetting : public Setting
{
public:
    PolygonSetting();
    ~PolygonSetting() {}




public:
    virtual void loadFromJsonObject(const QJsonObject& object);
    virtual void saveAsJsonObject();



public:
    bool isClosed() const;
    void clear();
    void pop_back();
    /**
     * @brief 闭合Poly
     */
    void setClosed();
    void addPoint(const QPointF& point);
    void setPoint(int index, const QPointF& point);
    /**
     * @brief 设置末尾点
     * @param point
     */
    void setBackPoint(const QPointF& point);
    int size() const {return polygon_.size();}


    int selectPointId(const QPointF& point);
    const QPolygonF& polygon() const {return polygon_;}
    QPolygonF& polygon() {return polygon_;}

public:
    QColor color() const;

private:
    QPolygonF polygon_;
    QColor color_;

};
}
#endif // POLYGONSETTING_H
