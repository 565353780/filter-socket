#ifndef POLYGONSETTER_H
#define POLYGONSETTER_H
#include "Setter.h"
namespace MaskConfig {
class PolygonSetting;
/**
 * @brief 多边形框(限一)"polygon"元素
 */
class PolygonSetter : public Setter
{
public:
    PolygonSetter(Setting* setting);


public:
    int runCommand(const QString &command);

     void init(int state = 1);

     int mouseMove(int posX, int posY);
     int mousePress(int posX,int posY, unsigned int button, unsigned int modifier = 0);
     int mouseRelease(int posX, int posY, unsigned int button, unsigned int modifier = 0);

     int paint(QPainter &painter);

private:
    PolygonSetting *poly_setting_;
    int current_point_id_;

};
}
#endif // POLYGONSETTER_H
