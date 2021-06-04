#include "BBoxSetter.h"
#include "Config/Setting/BBoxSetting.h"
#include "SetterCommands.h"
#include <QPainter>
namespace MaskConfig {

BBoxSetter::BBoxSetter(Setting *setting):PolygonsSetter(setting),bbox_setting_(NULL),tracking_id_(-1)
{
    bbox_setting_ = dynamic_cast<BBoxSetting * >(setting_);
    command_list_.clear();
    command_list_.push_back(Command(SetterCommands::Clear,1));

    command_list_.push_back(Command(SetterCommands::Next,1));
    command_list_.push_back(Command(SetterCommands::Prev,1));
    command_list_.push_back(Command(SetterCommands::ShowMode,1));


//    command_list_.push_back(Command(SetterCommands::Add,1));
    command_list_.push_back(Command(SetterCommands::Remove,1));

    if(this->is_tracking())
        command_list_.push_back(Command(SetterCommands::SetNextId,1));
//    command_list_.push_back(Command(SetterCommands::SetTrack,1));

//    command_list_.push_back(Command(SetterCommands::PopPoly,1));
}

void BBoxSetter::init(int state)
{
    if(state == 0 || state == 2)
    {
        this->initUndo();
    }

    selected_point_id_ = -1;

    if(this->setTrack(tracking_id_))
    {

    }
    else if(polys_setting_)
    {
       if(!this->isPolyIdVaild())
            current_id_ = polys_setting_->polygons_.size() - 1;
    }
    else
    {
        current_id_ = -1;
    }
//    if(current_id_<0)
//    {
//        setting_->update(0);
//    }
    polyChanged();
}

void BBoxSetter::finished()
{
    this->setTrackCurrent();
}

int BBoxSetter::mouseMove(int posX, int posY)
{
    if(posX < 0) posX = 0;
    if(posY < 0) posY = 0;
    if(posX >= i_size_.width()) posX = i_size_.width() - 1;
    if(posY >= i_size_.height()) posY = i_size_.height() - 1;

    current_mouse_point_.setX(posX);
    current_mouse_point_.setY(posY);

    QPointF p = this->VideoPosFromScreenPos(current_mouse_point_);
    if(modifier_ == Qt::ControlModifier)
    {
        if(button_ == Qt::LeftButton)
        {
            if(this->selected_point_id_<0) return 0;
            this->setBBox(p,last_point_);
        }
        else if(button_ == Qt::RightButton)
        {
            if(p.y() > this->last_point_.y())
            {
                this->scaleBBox(QPointF(1,1));
            }
            else if(p.y() < this->last_point_.y())
            {
                this->scaleBBox(QPointF(-1,-1));
            }

            this->last_point_ = p;
        }
    }
    else
    {
        if(button_ == Qt::LeftButton)
        {
            this->setBBox(p,last_point_);
        }
        else if(button_ == Qt::RightButton)
        {
            if(current_id()>=0 && min_id_>=0)
            {
                this->moveBBox(p-last_point_);
                last_point_ = p;
            }
        }
    }
    return 0;
}

int BBoxSetter::mousePress(int posX, int posY, unsigned int button, unsigned int modifier)
{
    button_ = button;
    modifier_ = modifier;
    QPointF p = this->VideoPosFromScreenPos(QPointF(posX,posY));
    //    qDebug()<<p;
    if(modifier_ == Qt::ControlModifier)
    {
        if(button_ == Qt::LeftButton)
        {
            selected_point_id_ = this->selectPointId(p);
            last_point_ = this->getFixedPoint();
        }
        else if(button_ == Qt::RightButton)
        {
            selectBBox(p);
            this->last_point_ = p;
        }
    }
    else
    {
        if(button_ == Qt::LeftButton)
        {

            this->add();
            this->setBBox(p,p);
            bbox_setting_->track_ids_.back() = bbox_setting_->next_id_+1;

            last_point_ = p;
            this->updateInfo();
        }
        else if(button_ == Qt::RightButton)
        {
            selectBBox(p);
            this->last_point_ = p;
        }
    }
    return 0;
}

int BBoxSetter::mouseRelease(int posX, int posY, unsigned int button, unsigned int modifier)
{

    if(button_ == Qt::LeftButton)
    {
        if(modifier_ == Qt::NoModifier && polys_setting_->polygons_.size() > 0)
        {
            QPolygonF polygon = polys_setting_->polygons_.back();
            if(polygon.at(0) == polygon.at(2))
            {
                this->pop_poly();
                current_id_ = -1;
            }
            else
            {
                this->bbox_setting_->next_id_++;
            }

        }

        this->addUndo();
    }
    else if(button_ == Qt::RightButton)
    {
        this->addUndo();
    }

    button_ = Qt::NoButton;
    modifier_ = Qt::NoModifier;
    selected_point_id_ = -1;
    return 0;
}

int BBoxSetter::keyUp(const unsigned int& key, unsigned int modifier)
{
    if(key == Qt::Key_Control)
    {
        is_scale_img_ = true;
    }
    return 0;
}

int BBoxSetter::keyDown(const unsigned int& key, unsigned int modifier)
{
    if(key == Qt::Key_Control)
    {
        is_scale_img_ = false;
    }
    return 0;
}

int BBoxSetter::wheel(const float &delta)
{
    if(is_scale_img_)
    {
        if(delta>0)
        {
            this->scaleImage(0.9);
        }
        else
        {
            this->scaleImage(1.1);
        }
    }
    else
    {
        if(current_id_<0)
        {
            return 0;
        }

        if(delta>0)
        {
            this->scaleBBox(QPointF(1,1));
        }
        else
        {
            this->scaleBBox(QPointF(-1,-1));
        }
    }

    return 0;
}

int BBoxSetter::runCommand(const QString &command)
{
    if(command == SetterCommands::SetTrack)
    {
        this->setTrackCurrent();
        this->showAll_ = false;
    }
    else if(command == SetterCommands::SetNextId)
    {
        this->bbox_setting_->set_next_id();
        this->bbox_setting_->update();
    }
    return PolygonsSetter::runCommand(command);
}

void BBoxSetter::updateInfo()
{
    if(this->is_tracking())
    {
        this->info_list_.clear();
        for(int i=0; i < this->polys_setting_->polygons_.size(); i++)
        {
            QString text ="ID:"+ QString::number(this->bbox_setting_->track_ids_[i]);

            text += " " + this->polys_setting_->label_dict_list_[0]->option(polys_setting_->labels_[i][0]);

            this->info_list_.push_back(text);
        }

        this->polys_setting_->updateInfo();
    }
    else
    {
        PolygonsSetter::updateInfo();
    }
}

int BBoxSetter::paint(QPainter &painter)
{
    if(!polys_setting_) return 0;
    this->paintStart(painter);
    int pw = 3 * v_size_.width() / i_size_.width();

    if(pw < 1)
    {
        pw = 1;
    }
    QFont font = painter.font();
    font.setPixelSize(pw*9);
    font.setBold(true);
    painter.setFont(font);

    if(showAll_)
    {
        for(int id=0; id < polys_setting_->polygons_.size(); id++)
        {
            QPen pen = painter.pen();
            pen.setWidth(pw);
            pen.setColor(polys_setting_->color());
            painter.setPen(pen);
            if(showAll_ && id != current_id_)
            {
                QPolygonF current_polygon = trans.inverted().map(polys_setting_->polygons_[id]);

                painter.drawPolyline(current_polygon);
                QRectF rect = current_polygon.boundingRect();
                painter.drawText(rect," " + QString::number(bbox_setting_->track_ids_[id]) + " " + this->polys_setting_->label_dict_list_[0]->option(polys_setting_->labels_[id][0]));
            }
        }
        tracking_id_ = -1;
    }
    if(isPolyIdVaild())
    {
        QPen pen = painter.pen();
        pen.setWidth(pw);
        pen.setColor(polys_setting_->selectedColor());
        painter.setPen(pen);

        QPolygonF current_polygon = trans.inverted().map(polys_setting_->polygons_[current_id_]);

        painter.drawPolyline(current_polygon);
        QRectF rect = current_polygon.boundingRect();
        painter.drawText(rect," "+QString::number(bbox_setting_->track_ids_[current_id_]) + " " + this->polys_setting_->label_dict_list_[0]->option(polys_setting_->labels_[current_id_][0]));
        //        for(int i=0; i < polygon.size(); i++)
        //        {
        //            QPointF p = polygon.at(i);
        //            painter.drawRect(p.x()-1,p.y()-1,2,2);
        //        }

    }

    this->paintEnd(painter);
    return 0;
}

void BBoxSetter::setBBox(QPointF p0, QPointF p1)
{
    if(!this->isPolyIdVaild()) return;
    QPointF pmin,pmax;
    pmin.rx() = p0.x() < p1.x()?p0.x():p1.x();
    pmin.ry() = p0.y() < p1.y()?p0.y():p1.y();
    pmax.rx() = p0.x() > p1.x()?p0.x():p1.x();
    pmax.ry() = p0.y() > p1.y()?p0.y():p1.y();
    QPolygonF polygon;
    polygon.append(pmin);
    polygon.append(QPointF(pmin.rx(),pmax.ry()));
    polygon.append(QPointF(pmax));
    polygon.append(QPointF(pmax.rx(),pmin.ry()));
    polygon.append(pmin);
    polys_setting_->polygons_[current_id_] = polygon;

}

void BBoxSetter::moveBBox(QPointF mv)
{
    if(current_id_ < 0) return;
    polys_setting_->polygons_[current_id_].translate(mv);
}

void BBoxSetter::scaleBBox(QPointF sv)
{
    if(!this->isPolyIdVaild()) return;
    QPointF p0 = polys_setting_->polygons_[current_id_].at(0);
    QPointF p1 = polys_setting_->polygons_[current_id_].at(2);
    QPointF pmin,pmax;
    pmin.rx() = p0.x() < p1.x()?p0.x():p1.x();
    pmin.ry() = p0.y() < p1.y()?p0.y():p1.y();
    pmax.rx() = p0.x() > p1.x()?p0.x():p1.x();
    pmax.ry() = p0.y() > p1.y()?p0.y():p1.y();
    pmin-=sv;
    pmax+=sv;
    this->setBBox(pmin,pmax);
}

void BBoxSetter::scaleImage(double scale_milti)
{
    double current_img_show_width = img_show_size_.width();
    double current_img_show_height = img_show_size_.height();

    QPointF current_mouse_point_on_img = img_start_point_ + QPointF(current_img_show_width * current_mouse_point_.x() / i_size_.width(), current_img_show_height * current_mouse_point_.y() / i_size_.height());

    QPointF current_left_top_dist = current_mouse_point_on_img - img_start_point_;

    double true_scale_milti = scale_milti;

    if(current_img_show_width * scale_milti > img_max_size_.width())
    {
        true_scale_milti = fmin(true_scale_milti, img_max_size_.width() / current_img_show_width);
    }
    if(current_img_show_height * scale_milti > img_max_size_.height())
    {
        true_scale_milti = fmin(true_scale_milti, img_max_size_.height() / current_img_show_height);
    }

    img_show_size_.setWidth(current_img_show_width * true_scale_milti);
    img_show_size_.setHeight(current_img_show_height * true_scale_milti);

    img_start_point_ = current_mouse_point_on_img - current_left_top_dist * true_scale_milti;

    if(img_start_point_.x() < 0)
    {
        img_start_point_.setX(0);
    }
    else if(img_start_point_.x() + img_show_size_.width() >= img_max_size_.width())
    {
        img_start_point_.setX(img_max_size_.width() - img_show_size_.width());
    }

    if(img_start_point_.y() < 0)
    {
        img_start_point_.setY(0);
    }
    else if(img_start_point_.y() + img_show_size_.height() >= img_max_size_.height())
    {
        img_start_point_.setY(img_max_size_.height() - img_show_size_.height());
    }

    updateTrans();
}

QPointF BBoxSetter::getFixedPoint()
{
    if(!this->isPolyIdVaild() || selected_point_id_ < 0) return QPointF();
    if(selected_point_id_ == 4) selected_point_id_ = 0;
    int id = (selected_point_id_+2) % 4;
    return polys_setting_->polygons_[current_id_].at(id);
}


bool BBoxSetter::is_tracking() const
{
    return bbox_setting_->is_tracking_;
}

void BBoxSetter::selectBBox(QPointF p0)
{
    if(!this->showAll_)
    {
        min_id_ = current_id_;
        return;
    }
    QPointF pmin,pmax;
    float rdist = 30;
    pmin.rx() = p0.x() - rdist;
    pmin.ry() = p0.y() - rdist;
    pmax.rx() = p0.x() + rdist;
    pmax.ry() = p0.y() + rdist;

    QPolygonF t_polygon;
    t_polygon.append(pmin);
    t_polygon.append(QPointF(pmin.rx(),pmax.ry()));
    t_polygon.append(QPointF(pmax));
    t_polygon.append(QPointF(pmax.rx(),pmin.ry()));
    t_polygon.append(pmin);

    float mindist = -1;
    int minid = -1;

    for(int i=0; i < polys_setting_->polygons_.size(); i++)
    {
        const QPolygonF& polygon = polys_setting_->polygons_[i];
        QPolygonF ip = t_polygon.intersected(polygon);

        if(ip.size() >= 4)
        {
            QPointF pp = ip.at(2)  - ip.at(0);
            float dist = fabs(pp.x())+fabs(pp.y());
            if(dist < rdist*4)
            {
                if(mindist < 0 || mindist > dist)
                {
                    mindist = dist;
                    minid = i;
                }
            }
        }
    }
    min_id_ = minid;
    if(minid >= 0 && minid != current_id_)
    {
        current_id_ = minid;
        this->polyChanged();
    }
}

bool BBoxSetter::setTrack(int tid)
{
    tracking_id_ = tid;
    if(tracking_id_>=0)
    {
        current_id_ = -1;
        for(int i=0; i < bbox_setting_->track_ids_.size(); i++)
        {
            if(bbox_setting_->track_ids_[i] == tracking_id_)
            {
                current_id_ = i;
            }
        }
        return true;
    }
    else
    {
        return false;
    }
}

void BBoxSetter::setTrackCurrent()
{
    if(isPolyIdVaild())
    {
        this->setTrack(this->bbox_setting_->track_ids_[current_id_]);
    }
}

}
