#ifndef SETTINGFUNCTIONS_H
#define SETTINGFUNCTIONS_H
#include <cmath>

class QJsonArray;
class QJsonValue;
class QPolygonF;
class QPointF;
class QJsonObject;
class QColor;
namespace MaskConfig{
namespace Functions {
    bool getJsonArrayFromPolygon(QJsonArray& array, const QPolygonF& polygon);
    bool getPolygonFromJsonArray(QPolygonF& polygon, const QJsonArray& points);
    bool getPolygonFromJsonValue(QPolygonF& polygon, const QJsonValue& val);


    void getColorFromJsonObject(QColor& color, const QJsonObject& color_object);

    double pointDist(const QPointF& p0, const QPointF& p1);
}
}
#endif // SETTINGFUNCTIONS_H
