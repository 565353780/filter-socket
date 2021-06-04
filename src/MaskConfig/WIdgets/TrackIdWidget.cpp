#include "TrackIdWidget.h"
#include <QLabel>
#include <QLineEdit>
#include <QIntValidator>
#include "Config/Setter/BBoxSetter.h"
#include "Config/Setting/BBoxSetting.h"
namespace MaskConfig {

TrackIdWidget::TrackIdWidget(BBoxSetter *bbox_setter, QWidget *parent):QWidget(parent),bbox_setter_(bbox_setter)
{
    next_id_label_ = new QLabel(tr("NextId"),this);
    id_label_ = new QLabel(tr("Id"),this);


    next_id_edit_ = new QLineEdit(this);
    next_id_edit_->setValidator(new QIntValidator(0,999999,next_id_edit_));
    id_edit_ = new QLineEdit(this);
    id_edit_->setValidator(new QIntValidator(0,999999,id_edit_));


    resetGeometry();

    connect(get_setting(),SIGNAL(signalUpdate(int)),this,SLOT(slotUpdate(int)));
    connect(next_id_edit_,SIGNAL(textChanged(QString)),this,SLOT(slotNextIdChanged(QString)));
    connect(id_edit_,SIGNAL(textChanged(QString)),this,SLOT(slotIdChanged(QString)));



}

void TrackIdWidget::slotNextIdChanged(const QString &str)
{
    int next_id = str.toInt();
    this->get_setting()->next_id_ = next_id;
}

void TrackIdWidget::slotIdChanged(const QString &str)
{
    int id = str.toInt();
    int current_id = bbox_setter_->current_id();
    if(current_id<0) return;
//    get_setting()->track_ids_[current_id] = id;
}

void TrackIdWidget::slotUpdate(int t)
{
    setNextId(this->get_setting()->next_id_);
    int current_id = bbox_setter_->current_id();
    int id = 0;
    if(current_id<0){
        id = 0;
    }
    else
    {
        id =  get_setting()->track_ids_[current_id];
    }
    if(id == id_edit_->text().toInt()) return;
    id_edit_->setText(QString::number(id));
}

void TrackIdWidget::setNextId(int id)
{
    if(id == next_id_edit_->text().toInt()) return;
    next_id_edit_->setText(QString::number(id));
}

void TrackIdWidget::resetGeometry()
{
    next_id_label_->move(15,8);
    next_id_edit_->move(100,0);

    id_label_->move(15,38);
    id_edit_->move(100,30);
}

BBoxSetting *TrackIdWidget::get_setting()
{
    return bbox_setter_->bbox_setting_;
}


}
