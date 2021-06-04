#include "PolygonsSetter.h"
#include "Config/Setting/PolygonListSetting.h"
#include "SetterCommands.h"
#include "Config/Setting/SettingFunctions.h"
#include "Config/Setting/LabelDict.h"
#include "PolygonsBinding.h"
#include "../Dict.h"
#include <QPainter>
#include <QDebug>
namespace MaskConfig {

PolygonsSetter::PolygonsSetter(Setting *setting): Setter(setting),selected_point_id_(-1),showAll_(true)
{
    polys_setting_ = dynamic_cast<PolygonListSetting * >(setting_);
    command_list_.push_back(Command(SetterCommands::Clear,1));
    command_list_.push_back(Command(SetterCommands::PopPoint,1));
    command_list_.push_back(Command(SetterCommands::SetClosed,1));

    command_list_.push_back(Command(SetterCommands::Next,1));
    command_list_.push_back(Command(SetterCommands::Prev,1));
    command_list_.push_back(Command(SetterCommands::ShowMode,1));


    command_list_.push_back(Command(SetterCommands::Add,1));
    command_list_.push_back(Command(SetterCommands::Remove,1));
    command_list_.push_back(Command(SetterCommands::PopPoly,1));




}

void PolygonsSetter::setVideoSize(int w, int h)
{
    Setter::setVideoSize(w,h);
    polys_setting()->v_size_ = this->v_size_;
}

int PolygonsSetter::runCommand(const QString &command)
{

    if(command == SetterCommands::Clear)
    {
        this->clear();
    }
    else if(command == SetterCommands::PopPoint)
    {
        this->pop_point();
    }
    else if(command == SetterCommands::SetClosed)
    {
        this->set_closed();
    }
    else if(command == SetterCommands::Next)
    {
        this->next();
    }
    else if(command == SetterCommands::Prev)
    {
        this->prev();
    }
    else if(command == SetterCommands::ShowMode)
    {
        this->showMode();
    }
    else if(command == SetterCommands::Add)
    {
        this->add();
    }
    else if(command == SetterCommands::Remove)
    {
        this->remove();
    }
    else if(command == SetterCommands::PopPoly)
    {
        this->pop_poly();
    }
    this->addUndo();
    return 0;
}

void PolygonsSetter::pop_poly()
{
    if(!polys_setting_) return;

    polys_setting_->pop_back();
    current_id_ = polys_setting_->polygons_.size() - 1;
    polyChanged();


}

void PolygonsSetter::remove()
{
    if(!polys_setting_) return;
    if(current_id_ >= 0 && current_id_ < polys_setting_->polygons_.size())
    {
        polys_setting_->remove(this->current_id_);
        current_id_--;
        polyChanged();

    }
}

void PolygonsSetter::add()
{
    if(!polys_setting_) return;

    polys_setting_->add(QPolygonF());
    current_id_ = polys_setting_->polygons_.size() - 1;
//    LogTool::debug(QString::number(current_id_));
    polyChanged();


}

void PolygonsSetter::showMode()
{
    showAll_ = !showAll_;
}

void PolygonsSetter::next()
{
    if(!polys_setting_) return;
    if(polys_setting_->polygons_.size() == 0) return;
    current_id_ = (current_id_ + 1) % (polys_setting_->polygons_.size());
    polyChanged();
}

void PolygonsSetter::prev()
{
    if(!polys_setting_) return;
    if(polys_setting_->polygons_.size() == 0) return;

    current_id_ = (current_id_+ polys_setting_->polygons_.size() - 1) % (polys_setting_->polygons_.size());
    polyChanged();

}

void PolygonsSetter::clear()
{
    if(!polys_setting_) return;
    polys_setting_->clear();
    current_id_ = -1;
    polyChanged();

}

void PolygonsSetter::pop_point()
{
    if(!this->isPolyIdVaild()) return;
    polys_setting_->polygons_[current_id_].pop_back();
}

void PolygonsSetter::set_closed()
{
    if(!this->isPolyIdVaild()) return;
    QPolygonF& poly = polys_setting_->polygons_[current_id_];
    if(poly.size() >= 3)
    {
        QPointF pfirst = poly.first();
        QPointF pBack = poly.back();
        if(Functions::pointDist(pfirst,pBack) < 10.0)
        {
            poly.back() = poly.first();
        }
        else
        {
            poly.push_back(poly.first());
        }
    }
    this->polyChanged();
}

void PolygonsSetter::set_current_id(int current_id)
{
    current_id_ = current_id;
    if(current_id_ < polys_setting_->polygons_.size())
    {
        polyChanged();
    }
    else
    {
        current_id_ = -1;
    }
}


void PolygonsSetter::addPoint(const QPointF &point)
{
    if(!this->isPolyIdVaild()) return;
    QPolygonF& poly = polys_setting_->polygons_[current_id_];

    if(poly.size()>=3 && poly.isClosed())
    {
        return;
    }
    poly.push_back(point);

}

void PolygonsSetter::setPoint(int index, const QPointF &point)
{
    if(!this->isPolyIdVaild()) return;
    QPolygonF& poly = polys_setting_->polygons_[current_id_];
    if(index < 0 || index >= poly.size()) return;

    if(poly.size()>=3 && poly.isClosed())
    {
        if(index == 0 || index == poly.size() - 1)
        {
            poly.first() = point;
            poly.back() = point;
        }
        else
        {
             poly[index] = point;
        }
    }
    else
    {
         poly[index] = point;
    }
}

void PolygonsSetter::setBackPoint(const QPointF &point)
{
    if(!this->isPolyIdVaild()) return;

    QPolygonF& poly = polys_setting_->polygons_[current_id_];
    if(poly.size()==0) return;
    if(poly.size()>=3 && poly.isClosed())
    {
        return;
    }
    poly.back() = point;
}

int PolygonsSetter::selectPointId(const QPointF &point)
{
    if(!this->isPolyIdVaild()) return -1;
    QPolygonF& polygon = polys_setting_->polygons_[current_id_];

    int id = -1;
    double mindist = -1;
    for(int i=0; i < polygon.size(); i++)
    {
        QPointF p = polygon.at(i);
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

int PolygonsSetter::pointSize(int id) const
{
    if(id < 0)
    {
        id = current_id_;
    }
    if(id < 0 || id >= polys_setting_->polygons_.size()) return 0;
    int size = polys_setting_->polygons_[id].size();
    if(size > 3 && polys_setting()->polygons_[id].isClosed())
    {
        return size - 1;
    }
    return size;

}

void PolygonsSetter::init(int state)
{
    if(state == 0 || state == 2)
    {
        this->initUndo();
    }

    selected_point_id_ = -1;

    if(polys_setting_)
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

int PolygonsSetter::mouseMove(int posX, int posY)
{
//    if(posX < 0 || posY < 0 || posX >= i_size_.width() || posY >= i_size_.height()) return 0;
    if(posX < 0) posX = 0;
    if(posY < 0) posY = 0;
    if(posX >= i_size_.width()) posX = i_size_.width() - 1;
    if(posY >= i_size_.height()) posY = i_size_.height() - 1;
    QPointF p = this->VideoPosFromScreenPos(QPointF(posX,posY));
    if(modifier_ == Qt::ControlModifier)
    {
        if(button_ == Qt::LeftButton)
        {
            this->setPoint(selected_point_id_,p);
        }
    }
    else
    {
        if(button_ == Qt::LeftButton)
        {
             this->setBackPoint(p);
        }
    }
    return 0;
}

int PolygonsSetter::mousePress(int posX, int posY, unsigned int button, unsigned int modifier)
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
        }
    }
    else
    {
        if(button_ == Qt::LeftButton)
        {
            if(this->polys_setting_->polygons_.size() == 0)
            {
                this->add();
            }
            this->addPoint(p);
            this->updateInfo();
        }
    }
    return 0;
}

int PolygonsSetter::mouseRelease(int posX, int posY, unsigned int button, unsigned int modifier)
{


    if(button_ == Qt::RightButton)
    {
       this->set_closed();
    }
    this->addUndo();

    button_ = Qt::NoButton;
    modifier_ = Qt::NoModifier;
    selected_point_id_ = -1;
    return 0;
}

int PolygonsSetter::keyDown(const unsigned int &key, unsigned int modifier)
{
    if(key == Qt::Key_Right)
    {
        next();
    }
    else if(key == Qt::Key_Left)
    {
        prev();
    }
    else if(key == Qt::Key_A)
    {
        add();
        this->addUndo();
    }
    else if(key == Qt::Key_Delete)
    {
        remove();
        this->addUndo();
    }
    else if(key == Qt::Key_P)
    {
        pop_point();
        this->addUndo();
    }
    else if(key == Qt::Key_Z && modifier == Qt::ControlModifier)
    {
        undo();
    }
    else if(key == Qt::Key_Y && modifier == Qt::ControlModifier)
    {
        redo();
    }
    return 0;
}

int PolygonsSetter::paint(QPainter &painter)
{
    if(!polys_setting_) return 0;
    this->paintStart(painter);

    for(int i=0; i < polys_bindings_.size(); i++)
    {
        polys_bindings_[i]->paint(painter);
    }
    int pw = 4 * v_size_.width() / i_size_.width();
    if(pw < 1)
    {
        pw = 1;
    }
    QFont font = painter.font();
    font.setPixelSize(pw*9);
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
                painter.drawPolyline(polys_setting_->polygons_[id]);
            }
        }
    }
    if(isPolyIdVaild())
    {
        QPen pen = painter.pen();
        pen.setWidth(pw);
        pen.setColor(polys_setting_->selectedColor());
        painter.setPen(pen);
        QPolygonF& polygon = polys_setting_->polygons_[current_id_];
        painter.drawPolyline(polygon);

//        for(int i=0; i < polygon.size(); i++)
//        {
//            QPointF p = polygon.at(i);
//            painter.drawRect(p.x()-1,p.y()-1,2,2);
//        }

    }
    return 0;
}

void PolygonsSetter::update()
{
    if(!isPolyIdVaild()) return;
    for(int i=0; i < this->polys_setting_->label_dict_list_.size(); i++)
    {
        LabelDict* dict = this->polys_setting_->label_dict_list_[i].get();
        this->polys_setting_->labels_[current_id_][i] = dict->key_;
    }
}

bool PolygonsSetter::isPolyIdVaild() const
{
    if(!polys_setting_) return false;
    if(current_id_<0 || current_id_ >= polys_setting_->polygons_.size()) return false;
    return true;
}

void PolygonsSetter::polyChanged()
{
    if(this->polys_setting_->labels_.size() > 0)
    {
//        qDebug() << "PolygonsSetter::polyChanged : Fix the first 2 bboxs' labels to 0 and 2.";
//        if(this->polys_setting_->labels_[0][0] != "0")
//        {
//            this->polys_setting_->labels_[0][0] = "0";
//        }
//        if(this->polys_setting_->labels_.size() > 1)
//        {
//            if(this->polys_setting_->labels_[1][0] != "2")
//            {
//                this->polys_setting_->labels_[1][0] = "2";
//            }
//        }

//        qDebug() << "PolygonsSetter::polyChanged : Fix the first bbox's labels to 1.";
//        if(this->polys_setting_->labels_[0][0] != "1")
//        {
//            this->polys_setting_->labels_[0][0] = "1";
//        }
    }

    this->updateInfo();

    for(int i=0; i < this->polys_setting_->label_dict_list_.size(); i++)
    {
        LabelDict* dict = this->polys_setting_->label_dict_list_[i].get();
        if(isPolyIdVaild())
        {
            dict->key_ = this->polys_setting_->labels_[current_id_][i];
        }
        else
        {
            if(dict->option_keys_.size() > 0)
                dict->key_ = dict->option_keys_.first();
        }
    }

    this->setting()->update();
}

void PolygonsSetter::updateInfo()
{
//    current_id_ = current_id_;
    this->info_list_.clear();
    for(int i=0; i < this->polys_setting_->polygons_.size(); i++)
    {
        QString text = QString::number(i+1)+":";
        text += Dict::t(SetterWords::PointsNum)+" "+ QString::number(this->pointSize(i));
        if(polys_bindings_.size() > 0)
        {
            for(int j=0; j < polys_bindings_.size(); j++)
            {
               int id = (polys_bindings_[j]->getBindId(i));
               QString idText = QString::number(id);
               if(id < 0)
                {
                   idText = Dict::t(SetterWords::None);
               }
               text+="; "+polys_bindings_[j]->ui_name()+" "+idText;
            }
        }

        this->info_list_.push_back(text);
    }
    this->polys_setting_->updateInfo();

}

void PolygonsSetter::undo()
{
    if(this->undo_list_.size() > 1)
    {
        this->redo_list_.push_back(this->undo_list_.back());
        this->polys_setting_->loadFromJsonObject(this->undo_list_[this->undo_list_.size() - 2]);
        this->undo_list_.pop_back();

        if(!this->isPolyIdVaild())
        {
            this->current_id_ = -1;
        }
        this->updateInfo();
        this->setting()->update();
    }
}

void PolygonsSetter::redo()
{

    if(this->redo_list_.size() > 0)
    {
        this->undo_list_.push_back(this->redo_list_.back());
        this->polys_setting_->loadFromJsonObject(this->redo_list_.back());
        this->redo_list_.pop_back();

        if(!this->isPolyIdVaild())
        {
            this->current_id_ = -1;
        }

        this->updateInfo();
        this->setting()->update();
    }
}

void PolygonsSetter::initUndo()
{
    this->polys_setting_->saveAsJsonObject();
    this->undo_list_.push_back(this->polys_setting_->object());
    this->redo_list_.clear();
}

void PolygonsSetter::addUndo()
{
    this->polys_setting_->saveAsJsonObject();
    if(this->undo_list_.size()>0)
    {
        if(this->undo_list_.back() == this->polys_setting_->object())
        {
            return;
        }
    }
    undo_list_.push_back(this->polys_setting_->object());
    redo_list_.clear();
}

PolygonListSetting *PolygonsSetter::polys_setting() const
{
    return polys_setting_;
}

void PolygonsSetter::addBinding(const QString &key, PolygonListSetting *setting)
{
    polys_bindings_.push_back(std::shared_ptr<PolygonsBinding>(new PolygonsBinding(key,this,setting)));
}
}
