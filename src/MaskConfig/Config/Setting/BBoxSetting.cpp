#include "BBoxSetting.h"
#include <QDebug>
#include "SettingWords.h"
#include <QJsonArray>
#include <QJsonObject>
namespace MaskConfig{

BBoxSetting::BBoxSetting():is_tracking_(true),next_id_(0)
{
    has_setter_ = true;
    has_labels_ = false;
    color_ = QColor(Qt::yellow);
    type_ = SettingTypes::BBox;
}

bool BBoxSetting::isReadyToSave()
{
    for(int i=0; i < labels_.size(); i++)
    {
        for(int j=0; j < labels_[i].size(); j++)
        {
            if(labels_[i][j]=="0")
            {
                //LogTool::warn(SettingInfo::EmptyLabelWarning);
                return false;
            }
        }
    }
    return true;
}

void BBoxSetting::add(const QPolygonF &poly)
{
    PolygonListSetting::add(poly);
    track_ids_.push_back(0);
}

void BBoxSetting::remove(int index)
{
    if(index < 0 ||index>=this->polygons_.size()) return;
    PolygonListSetting::remove(index);
    this->track_ids_.removeAt(index);
}

void BBoxSetting::clear()
{
    PolygonListSetting::clear();
    this->track_ids_.clear();
    next_id_ = 0;
}

void BBoxSetting::pop_back()
{
    if(this->polygons_.size() > 0)
    {
        PolygonListSetting::pop_back();
        track_ids_.pop_back();
    }
}

void BBoxSetting::set_next_id()
{
    int mi = 0;
    for(int i=0; i < this->track_ids_.size(); i++)
    {
        if(mi < this->track_ids_[i])
        {
            mi = this->track_ids_[i];
        }
    }
    next_id_ = mi;
}

QRectF BBoxSetting::getBBox(int i, int &id)
{
    if(i < 0 || i >= this->polygons_.size()) return QRectF();
    QPolygonF polygon = this->polygons_[i];
    id = this->track_ids_[i];
    return polygon.boundingRect();
}

void BBoxSetting::setBBox(int i, const QRectF &rect)
{
    QPointF tl = rect.topLeft();
    QPointF tr = rect.topRight();
    QPointF br = rect.bottomRight();
    QPointF bl = rect.bottomLeft();
    QPolygonF polygon;
    polygon.push_back(tl);
    polygon.push_back(tr);
    polygon.push_back(br);
    polygon.push_back(bl);
    polygon.push_back(tl);
    this->polygons_[i] = polygon;

}

void BBoxSetting::loadFromJsonObject(const QJsonObject &object)
{
    this->track_ids_.clear();
    PolygonListSetting::loadFromJsonObject(object);
    if(object.contains("track"))
    {
        is_tracking_ = true;
        QJsonObject track_object = object["track"].toObject();
        this->next_id_ = track_object["NextId"].toInt();
        QJsonArray track_array = track_object["Array"].toArray();
        if(track_array.size() == track_ids_.size())
        {
            for(int i=0; i< track_array.size(); i++)
            {
                track_ids_[i] = track_array[i].toInt();
            }
        }
        else
        {
//            LogTool::warn("TrackingConfigError!");

        }

    }
}

void BBoxSetting::saveAsJsonObject()
{
    PolygonListSetting::saveAsJsonObject();
    if(!this->is_tracking_) return;
    QJsonArray track_array;
    for(int i=0; i < this->track_ids_.size(); i++)
    {
        track_array.push_back(QJsonValue(this->track_ids_[i]));
    }
    QJsonObject track_object;
    track_object["NextId"] = this->next_id_;
    track_object["Array"] = track_array;
    object_["track"] = track_object;
}
}
