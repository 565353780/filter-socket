#include "PolygonSetter.h"
#include "Config/Setting/PolygonSetting.h"
#include "SetterCommands.h"
#include "qnamespace.h"

#include <QPainter>
namespace MaskConfig {

PolygonSetter::PolygonSetter(Setting *setting):Setter(setting)
{
    poly_setting_ = dynamic_cast<PolygonSetting * >(setting_);
    command_list_.push_back(Command(SetterCommands::Clear,1));
    command_list_.push_back(Command(SetterCommands::PopPoint,1));
    command_list_.push_back(Command(SetterCommands::SetClosed,1));


}

int PolygonSetter::runCommand(const QString &command)
{
    if(!poly_setting_) return 0;
    if(command == SetterCommands::PopPoint)
    {
        poly_setting_->pop_back();
    }
    else if(command == SetterCommands::Clear)
    {
        poly_setting_->clear();
    }
    else if(command == SetterCommands::SetClosed)
    {
        poly_setting_->setClosed();
    }
    return 0;
}

void PolygonSetter::init(int state)
{
    current_point_id_ = -1;
}


int PolygonSetter::mousePress(int posX, int posY, unsigned int button, unsigned int modifier)
{
    button_ = button;
    modifier_ = modifier;
    QPointF p = this->VideoPosFromScreenPos(QPointF(posX,posY));

    if(modifier_ == Qt::ControlModifier)
    {
        if(button_ == Qt::LeftButton)
        {
            if(poly_setting_)
            {
                current_point_id_ = poly_setting_->selectPointId(p);
             }
        }
    }
    else
    {
        if(button_ == Qt::LeftButton)
        {
            if(poly_setting_)
            {
                if(poly_setting_->isClosed())
                {

                }
                else
                {
                    poly_setting_->addPoint(p);
                }
            }

        }
    }
    return 0;
}
int PolygonSetter::mouseMove(int posX, int posY)
{
    QPointF p = this->VideoPosFromScreenPos(QPointF(posX,posY));
    if(modifier_ == Qt::ControlModifier)
    {
        if(button_ == Qt::LeftButton)
        {
            if(current_point_id_>=0)
            {
                if(poly_setting_)
                {
                     poly_setting_->setPoint(current_point_id_,p);
                }
            }

        }
    }
    else
    {
        if(button_ == Qt::LeftButton)
        {
            if(poly_setting_)
            {
                if(poly_setting_->isClosed())
                {

                }
                else
                {
                    poly_setting_->setBackPoint(p);
                }
            }
        }
    }
    return 0;
}

int PolygonSetter::mouseRelease(int posX, int posY, unsigned int button, unsigned int modifier)
{
    if(button_ == Qt::RightButton)
    {
        if(poly_setting_) poly_setting_->setClosed();
    }
    button_ = Qt::NoButton;
    modifier_ = Qt::NoModifier;
    current_point_id_ = -1;
    return 0;
}

int PolygonSetter::paint(QPainter &painter)
{

    this->paintStart(painter);


    if(poly_setting_)
    {
        QPen pen = painter.pen();
        pen.setWidth(8);
        pen.setColor(poly_setting_->color());
        painter.setPen(pen);
        painter.drawPolyline(poly_setting_->polygon());

        for(int i=0; i < poly_setting_->polygon().size(); i++)
        {
            QPointF p = poly_setting_->polygon().at(i);
            painter.drawRect(p.x()-2,p.y()-2,4,4);
        }
    }

    this->paintEnd(painter);

    return 0;

}


}
