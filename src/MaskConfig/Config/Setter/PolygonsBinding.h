#ifndef POLYGONSBINDING_H
#define POLYGONSBINDING_H
#include <QString>
class QPainter;
namespace MaskConfig {
class PolygonsSetter;
class PolygonListSetting;
/**
 * @brief 关联选框与id
 */
class PolygonsBinding
{
public:
    PolygonsBinding(const QString& key, PolygonsSetter* setter, PolygonListSetting* bind_setting);
    /**
     * @brief 转入下一个Polygons
     */
    void next();
    /**
     * @brief 转入上一个Polygons
     */
    void prev();

    void paint(QPainter &painter);
    QString ui_name() const;
public:
    /**
     * @brief 返回当前Polygon的id
     * @return
     */
    int getPolyId();
    /**
     * @brief 设置当前Polygon的id
     * @param id
     */
    void setPolyId(int id);
    PolygonsSetter *getSetter() const;


    int getBindId(int src_poly_id);

private:
    QString key_;
    PolygonListSetting *bind_setting_;
    PolygonsSetter* setter_;

};
}
#endif // POLYGONSBINDING_H
