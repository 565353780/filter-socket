#include "PolygonSetting.h"
#include "SettingWords.h"
#include "Util/LogTool.h"
#include "SettingFunctions.h"
#include <QJsonArray>
#include <QDebug>
namespace MaskConfig{

PolygonSetting::PolygonSetting()
{
    has_setter_ = true;
    color_ = QColor(Qt::red);
    type_ = SettingTypes::Polygon;
}

void PolygonSetting::loadFromJsonObject(const QJsonObject &object)
{
    Setting::loadFromJsonObject(object);
    QPolygonF p;
    bool polygon_error = true;
    if(object_.contains(SettingWords::Polygon))
    {
        polygon_error = false;
        polygon_error = !Functions::getPolygonFromJsonValue(p,object[SettingWords::Polygon]);
    }
    if(polygon_error)
    {
        LogTool::warn(SettingInfo::PolygonError);
    }

    polygon_ = p;

    if(object_.contains(SettingWords::Color))
    {
        QJsonValue val = object_[SettingWords::Color];
        if(val.isObject())
        {
            QJsonObject color_object = val.toObject();
            if(color_object.contains(SettingWords::R))
            {
                 color_.setRed(color_object[SettingWords::R].toInt());
            }
            if(color_object.contains(SettingWords::G))
            {
                 color_.setGreen(color_object[SettingWords::G].toInt());
            }
            if(color_object.contains(SettingWords::B))
            {
                 color_.setBlue(color_object[SettingWords::B].toInt());
            }
        }
    }

    this->update();

}

void PolygonSetting::saveAsJsonObject()
{
    object_ = QJsonObject();
    Setting::saveAsJsonObject();
    QJsonArray polygon_array;
    for(int i=0; i < polygon_.size(); i++)
    {
        QJsonArray pointJ;
        pointJ.push_back(QJsonValue( polygon_[i].x()));
        pointJ.push_back(QJsonValue( polygon_[i].y()));
        polygon_array.push_back(pointJ);
    }
    object_[SettingWords::Polygon] = polygon_array;

//    QJsonObject color_object;
//    color_object[SettingWords::R] = color_.red();
//    color_object[SettingWords::G] = color_.green();
//    color_object[SettingWords::B] = color_.blue();

//    object_[SettingWords::Color] = color_object;

}

bool PolygonSetting::isClosed() const
{
    return polygon_.size() >= 3 && polygon_.isClosed();
}

void PolygonSetting::clear()
{
    polygon_.clear();
}

void PolygonSetting::pop_back()
{
    if(polygon_.size() > 0)
    {
        polygon_.pop_back();
    }
}

void PolygonSetting::setClosed()
{
    if(polygon_.size() >= 3)
    {
        QPointF pfirst = polygon_.first();
        QPointF pBack = polygon_.back();
        if(Functions::pointDist(pfirst,pBack) < 10.0)
        {
            polygon_.back() = polygon_.first();
        }
        else
        {
            polygon_.push_back(polygon_.first());
        }
    }
}

void PolygonSetting::addPoint(const QPointF &point)
{
    polygon_.push_back(point);
}

void PolygonSetting::setPoint(int index, const QPointF &point)
{
    if(index<0 || index >= polygon_.size())
    {
        return;
    }
    if(this->isClosed() && (index==0 || index == polygon_.size()-1))
    {
        polygon_.back() = polygon_.first() = point;
    }
    else {
        polygon_[index] = point;
    }
}

void PolygonSetting::setBackPoint(const QPointF &point)
{
    if(polygon_.size() == 0) return;
    polygon_.back() = point;
}

int PolygonSetting::selectPointId(const QPointF &point)
{
    int id = -1;
    double mindist = -1;
    for(int i=0; i < polygon_.size(); i++)
    {
        QPointF p = polygon_.at(i);
        double dist = Functions::pointDist(p,point);
        if(dist < 15.0)
        {
            if(id < 0 || dist < mindist )
            {
                id = i;
                mindist = dist;
            }
        }
    }
    return id;

}

QColor PolygonSetting::color() const
{
    return color_;
}


}
