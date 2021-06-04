#include "InfoWidget.h"
#include <QPainter>
#include "Config/Setter/Setter.h"
#include "Config/Setting/Setting.h"
#include <QMouseEvent>
namespace MaskConfig {
InfoWidget::InfoWidget(Setter *setter, QWidget *parent):QWidget(parent),lineHeight_(25),setter_(setter)
{
    connect(setter->setting(),SIGNAL(signalInfoUpdate()),this,SLOT(resetGeometry()));
    connect(setter->setting(),SIGNAL(signalUpdate()),this,SLOT(update()));
}

void InfoWidget::resetGeometry()
{
    int w = 300;
    int h = lineHeight_ * setter_->info_list_.size();
    if(parent())
    {
        QWidget *pw = (QWidget*)parent();
        w = pw->width();
        if(h <pw->height())
        {
            h = pw->height();
        }

    }

    this->resize(w,h);

    this->update();
}

void InfoWidget::mouseReleaseEvent(QMouseEvent *e)
{
    int id = e->y() / lineHeight_;
    if(id >= setter_->info_list_.size())
    {
        return;
    }
    setter_->set_current_id(id);
    update();

}

void InfoWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);


    QFont font = painter.font();
    font.setPixelSize(lineHeight_ - 8);
    painter.setFont(font);
    painter.fillRect(0,0,this->width(),this->height(),QColor(240,240,240));


    int selected_line = setter_->current_id();
    painter.fillRect(0,selected_line * lineHeight_, this->width(),lineHeight_,QColor(200,200,255));


    for(int i=0; i < setter_->info_list_.size(); i++)
    {
        painter.drawText(5, i * lineHeight_+lineHeight_-5, setter_->info_list_[i]);
    }

}

}
