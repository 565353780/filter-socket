#include "Setter.h"
#include "Config/Setting/Setting.h"
#include <QPainter>
#include <QDebug>
namespace MaskConfig {
Setter::Setter(Setting *setting):setting_(setting),v_size_(1,1),i_size_(1,1),current_id_(-1),img_start_point_(0,0),img_show_size_(1,1),is_scale_img_(true)
{

}

QString Setter::name() const
{
    if(setting_)
    {
        return setting_->name();
    }
}

void Setter::setVideoSize(int w, int h)
{
    v_size_.setWidth(w);
    v_size_.setHeight(h);
}

int Setter::resize(int w, int h)
{
    i_size_.setWidth(w);
    i_size_.setHeight(h);
    return 0;
}


QPointF Setter::VideoPosFromScreenPos(QPointF p)
{
    QPointF point;
    point.rx() = p.x() * v_size_.width() / i_size_.width();
    point.ry() = p.y() * v_size_.height() / i_size_.height();

    point = trans.map(point);

    return point;
}

QPointF Setter::ScreenPosFromVideoPos(QPointF p)
{
    QPointF point;

    point = trans.inverted().map(point);

    point.rx() = p.x() / (double)v_size_.width() * i_size_.width();
    point.ry() = p.y() / (double)v_size_.height() * i_size_.height();
    return point;
}

void Setter::paintStart(QPainter &painter)
{
    painter.scale(i_size_.width() / v_size_.width(), i_size_.height() / v_size_.height());

}

void Setter::paintEnd(QPainter &painter)
{
    painter.scale(v_size_.width() / i_size_.width(), v_size_.height() / i_size_.height());
}

void Setter::updateTrans()
{
    trans.reset();
    trans.translate(img_start_point_.x(), img_start_point_.y());
    trans.scale(img_show_size_.width() / img_max_size_.width(), img_show_size_.height() / img_max_size_.height());
}

void Setter::set_current_id(int id)
{
    current_id_ = id;
}

int Setter::current_id() const
{
    return current_id_;
}

void Setter::setImageShowSize(QPointF img_start_point, QSizeF img_show_size)
{
    if(img_show_size.width() > 0 && img_show_size.height() > 0)
    {
        img_start_point_ = img_start_point;
        img_show_size_ = img_show_size;
        img_max_size_ = img_show_size;

        updateTrans();
    }
    else
    {
//        qDebug() << "give the wrong image show size.";
    }
}

void Setter::getImageShowSize(QPointF &img_start_point, QSizeF &img_show_size, QSizeF &img_max_size)
{
    img_start_point = img_start_point_;
    img_show_size = img_show_size_;
    img_max_size = img_max_size_;
}

}
