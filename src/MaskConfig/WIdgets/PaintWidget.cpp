#include "PaintWidget.h"
#include <QMouseEvent>
#include "ConfigWidget.h"
#include "Config/Setter/Setter.h"
#include <QPainter>
#include <QKeyEvent>

#include <QDebug>
namespace MaskConfig{

PaintWidget::PaintWidget(ConfigWidget *parent):QWidget(parent),config_widget_(parent)
{
    this->setMouseTracking(true);
}

void PaintWidget::resizeEvent(QResizeEvent *event)
{
    if(config_widget_->current_setter())
    {
        config_widget_->current_setter()->resize(this->width(),this->height());
    }
}

void PaintWidget::mousePressEvent(QMouseEvent *event)
{
    if(config_widget_->current_setter())
    {
        config_widget_->current_setter()->mousePress(event->pos().x(),event->pos().y(),event->buttons(),event->modifiers());
        this->update();
    }
    this->setFocus();
}

void PaintWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(config_widget_->current_setter())
    {
        config_widget_->current_setter()->mouseMove(event->pos().x(),event->pos().y());
        this->update();
    }
}

void PaintWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if(config_widget_->current_setter())
    {
        config_widget_->current_setter()->mouseRelease(event->pos().x(),event->pos().y(),event->buttons(),event->modifiers());
        this->update();
    }
}

void PaintWidget::keyPressEvent(QKeyEvent *event)
{
    if(config_widget_->current_setter())
    {
        config_widget_->current_setter()->keyDown(event->key(),event->modifiers());
        this->update();
    }
}

void PaintWidget::keyReleaseEvent(QKeyEvent *event)
{
    if(config_widget_->current_setter())
    {
        config_widget_->current_setter()->keyUp(event->key(),event->modifiers());
        this->update();
    }
}

void PaintWidget::wheelEvent(QWheelEvent *event)
{
    if(config_widget_->current_setter())
    {
        config_widget_->current_setter()->wheel(event->delta());
        this->update();
    }
}

void PaintWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    QPointF current_img_start_point;
    QSizeF current_img_show_size;
    QSizeF current_img_max_size;
    config_widget_->current_setter()->getImageShowSize(current_img_start_point, current_img_show_size, current_img_max_size);

    //painter.drawImage(QRectF(0,0,this->width(),this->height()),config_widget_->image(), QRectF(current_img_start_point, current_img_show_size));
    painter.drawImage(QRectF(0,0,this->width(),this->height()),config_widget_->image(), config_widget_->current_setter()->trans.mapRect(QRectF(0, 0, current_img_max_size.width(), current_img_max_size.height())));
    if(config_widget_->current_setter())
    {
        config_widget_->current_setter()->paint(painter);
    }
}

}
