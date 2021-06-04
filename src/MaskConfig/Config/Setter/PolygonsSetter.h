#ifndef POLYGONSSETTER_H
#define POLYGONSSETTER_H
#include "Setter.h"
#include <memory>

#include <QJsonDocument>
#include <QJsonParseError>
#include <QFile>
#include <QJsonObject>
#include <QDebug>
#include <QJsonArray>

namespace MaskConfig {
class PolygonsBinding;
class PolygonListSetting;
/**
 * @brief 多边形框(polygons)元素集合
 */
class PolygonsSetter : public Setter
{
public:
    PolygonsSetter(Setting* setting);


     void setVideoSize(int w, int h);
public:
    virtual int runCommand(const QString &command);

    void pop_poly();
    void remove();
    void add();

    /**
     * @brief 切换显示模式(全显，显示最新Poly)
     */
    void showMode();
    void next();
    void prev();


    virtual void clear();
    /**
     * @brief 删除当前点
     */
    void pop_point();
    /**
     * @brief 闭合选框
     */
    void set_closed();



    void set_current_id(int current_id);


    void addPoint(const QPointF& point);
    void setPoint(int index, const QPointF& point);
    /**
     * @brief 设置末尾点
     * @param point
     */
    void setBackPoint(const QPointF& point);
    /**
     * @brief 通过坐标近似选取点id
     * @param point
     * @return
     */
    int selectPointId(const QPointF& point);
    int pointSize(int id = -1) const;


public:


    virtual void init(int state = 1);
    virtual void finished() {}

    virtual int mouseMove(int posX, int posY);
    virtual int mousePress(int posX,int posY, unsigned int button, unsigned int modifier = 0);
    virtual int mouseRelease(int posX, int posY, unsigned int button, unsigned int modifier = 0);
    virtual int keyDown(const unsigned int &key, unsigned int modifier);
    virtual int wheel(const float &delta){return 0;}
    virtual int paint(QPainter &painter);

    void update();

    bool isPolyIdVaild() const;

    void polyChanged();

    virtual void updateInfo();


    void undo();
    void redo();
    void addUndo();

    PolygonListSetting *polys_setting() const;


    int bindingSize() const {return polys_bindings_.size();}
    PolygonsBinding* getBinding(int i) {return polys_bindings_[i].get();}
    void addBinding(const QString& key, PolygonListSetting* setting);

protected:
    void initUndo();


    QList<QJsonObject> undo_list_;
    QList<QJsonObject> redo_list_;

protected:
    QList< std::shared_ptr<PolygonsBinding> >  polys_bindings_;

    PolygonListSetting* polys_setting_;
    bool showAll_;
    int selected_point_id_;
};
}
#endif // POLYGONSSETTER_H
