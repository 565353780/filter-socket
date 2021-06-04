#ifndef BBOXSETTER_H
#define BBOXSETTER_H
#include "PolygonsSetter.h"
#include <cmath>
#include "../Setting/LabelDict.h"

namespace MaskConfig {
class BBoxSetting;
/**
 * @brief 矩形框"BBox"元素集合
 */
class BBoxSetter : public PolygonsSetter
{
public:
    BBoxSetter(Setting *setting);

    virtual void init(int state = 1);
    virtual void finished();

    virtual int mouseMove(int posX, int posY);
    virtual int mousePress(int posX,int posY, unsigned int button, unsigned int modifier = 0);
    virtual int mouseRelease(int posX, int posY, unsigned int button, unsigned int modifier = 0);
    virtual int keyUp(const unsigned int& key, unsigned int modifier = 0);
    virtual int keyDown(const unsigned int& key, unsigned int modifier = 0);
    virtual int wheel(const float &delta);

    /**
     * @brief 执行命令
     * @param command
     * @return
     */
    int runCommand(const QString &command);

    void updateInfo();
    int paint(QPainter &painter);

    /**
     * @brief 根据两点p0,p1生成BBox
     * @param p0
     * @param p1
     */
    void setBBox(QPointF p0, QPointF p1);
    /**
     * @brief 移动BBox至mv
     * @param mv
     */
    void moveBBox(QPointF mv);
    /**
     * @brief 放缩BBox sv
     * @param sv
     */
    void scaleBBox(QPointF sv);
    /**
     * @brief 放缩Image显示区域 sv
     * @param sv
     */
    void scaleImage(double scale_milti);
    /**
     * @brief 获取放缩操作时的固定点
     * @return
     */
    QPointF getFixedPoint();

    /**
     * @brief 是否在追踪
     * @return
     */
    bool is_tracking() const;

    /**
     * @brief 通过点p0选中BBox
     * @param p0
     */
    void selectBBox(QPointF p0);

    /**
     * @brief 追踪编号tid的BBox
     * @param tid
     * @return
     */
    bool setTrack(int tid);
    /**
     * @brief 追踪当前BBox
     */
    void setTrackCurrent();
public:
    BBoxSetting *bbox_setting_;

    int tracking_id_;
private:

    QPointF last_point_;
    int min_id_;
};
}
#endif // BBOXSETTER_H
