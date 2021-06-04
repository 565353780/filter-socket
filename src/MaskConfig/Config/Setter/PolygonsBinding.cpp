#include "PolygonsBinding.h"
#include "PolygonsSetter.h"
#include "../Setting/PolygonListSetting.h"
#include <QPainter>
#include <QDebug>
namespace MaskConfig {
PolygonsBinding::PolygonsBinding(const QString &key, PolygonsSetter *setter, PolygonListSetting *bind_setting):setter_(setter),bind_setting_(bind_setting),key_(key)
{

}

void PolygonsBinding::next()
{
    int id = this->getPolyId()+1;
    if(id >= bind_setting_->polygons_.size()){
        id = -1;
    }
    this->setPolyId(id);
    setter_->addUndo();

}

void PolygonsBinding::prev()
{
    int id = this->getPolyId()-1;
    if(id < -1){
        id = bind_setting_->polygons_.size() -1;
    }
    this->setPolyId(id);
    setter_->addUndo();

}

void PolygonsBinding::paint(QPainter &painter)
{
    int id = this->getPolyId();
    if(id < 0) return;
    QPen pen = painter.pen();
    pen.setWidth(8);
    pen.setColor(bind_setting_->selectedColor());
    QVector<qreal> dashes;
    qreal space = 3;
    dashes << 5 << space << 5 <<space;
    pen.setDashPattern(dashes);
    painter.setPen(pen);
    painter.drawPolyline(bind_setting_->polygons_[id]);
    painter.setPen(QPen());
}



QString PolygonsBinding::ui_name() const
{
    return bind_setting_->name();
}

int PolygonsBinding::getPolyId()
{
    if(setter_->isPolyIdVaild())
    {
        int id = setter_->polys_setting()->bindings_[setter_->current_id()][key_];
        if(id >= bind_setting_->polygons_.size()) {
            id = -1;
        }
        return id;
    }
    else
        return -1;

}

void PolygonsBinding::setPolyId(int id)
{
    if(setter_->isPolyIdVaild())
    {
        setter_->polys_setting()->bindings_[setter_->current_id()][key_] = id;
        setter_->setting()->update();
        setter_->setting()->updateInfo();
    }
}

PolygonsSetter *PolygonsBinding::getSetter() const
{
    return setter_;
}

int PolygonsBinding::getBindId(int src_poly_id)
{
   if(src_poly_id < 0 || src_poly_id >= setter_->polys_setting()->bindings_.size()) return -1;
   if(setter_->polys_setting()->bindings_[src_poly_id].contains(key_))
        return setter_->polys_setting()->bindings_[src_poly_id][key_];
   return -1;
}
}
