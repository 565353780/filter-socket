#ifndef BBOXSETTING_H
#define BBOXSETTING_H
#include "PolygonListSetting.h"
namespace MaskConfig{
/**
 * @brief 包含"type"为"BBox"的JsonObject的类
 */
class BBoxSetting : public PolygonListSetting
{
public:
    BBoxSetting();
    virtual ~BBoxSetting(){}

    virtual bool isReadyToSave();
public:
    void add(const QPolygonF& poly);
    void remove(int index);
    void clear();
    void pop_back();
    void set_next_id();  //设置下个BBox的id



    QRectF getBBox(int i, int &id);

    void setBBox(int i, const QRectF &rect);
    virtual void loadFromJsonObject(const QJsonObject& object);
    virtual void saveAsJsonObject();
public:
    int next_id_;
    bool  is_tracking_;
    QList<int> track_ids_;

};
}
#endif // BBOXSETTING_H
