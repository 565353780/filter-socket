#include "SettingFunctions.h"
#include <QJsonArray>
#include <QPolygon>
#include <QPoint>
#include <QJsonObject>
#include <QColor>
#include "SettingWords.h"
namespace MaskConfig{

bool Functions::getJsonArrayFromPolygon(QJsonArray &array, const QPolygonF &polygon)
{
    array = QJsonArray();
    for(int i=0; i < polygon.size(); i++)
    {
        QJsonArray pointJ;
        pointJ.push_back(QJsonValue( (int)polygon[i].x()));
        pointJ.push_back(QJsonValue( (int)polygon[i].y()));
        array.push_back(pointJ);
    }
    return true;
}

bool Functions::getPolygonFromJsonArray(QPolygonF &polygon, const QJsonArray &points)
{
    polygon = QPolygonF();
    bool polygon_error = false;
    for(int i=0; i < points.size(); i++)
    {
        if(points[i].isArray())
        {
            QJsonArray pointJ = points[i].toArray();
            if(pointJ.size() < 2)
            {
                 polygon_error = true;
            }
            else
            {
                 polygon.push_back(QPointF(pointJ[0].toDouble(),pointJ[1].toDouble()));
            }
        }
        else
        {
             polygon_error = true;
        }
    }
    return !polygon_error;
}

bool Functions::getPolygonFromJsonValue(QPolygonF &polygon, const QJsonValue &val)
{
    bool polygon_error = false;
    if(val.isArray())
    {
        QJsonArray points = val.toArray();
        for(int i=0; i < points.size(); i++)
        {
            if(points[i].isArray())
            {
                QJsonArray pointJ = points[i].toArray();
                if(pointJ.size() < 2)
                {
                     polygon_error = true;
                }
                else
                {
                     polygon.push_back(QPointF(pointJ[0].toDouble(),pointJ[1].toDouble()));
                }
            }
            else
            {
                 polygon_error = true;
            }
        }

    }
    else
    {
        polygon_error = true;
    }
    return !polygon_error;
}

double Functions::pointDist(const QPointF &p0, const QPointF &p1)
{
    return sqrt((p0.x() - p1.x())*(p0.x() - p1.x()) + (p0.y() - p1.y())*(p0.y() - p1.y()));
}

void Functions::getColorFromJsonObject(QColor &color, const QJsonObject &color_object)
{
    if(color_object.contains(SettingWords::R))
    {
         color.setRed(color_object[SettingWords::R].toInt());
    }
    if(color_object.contains(SettingWords::G))
    {
         color.setGreen(color_object[SettingWords::G].toInt());
    }
    if(color_object.contains(SettingWords::B))
    {
         color.setBlue(color_object[SettingWords::B].toInt());
    }
}


}
