#include "BindingWidget.h"
#include "Config/Setter/PolygonsBinding.h"
#include "Config/Setter/PolygonsSetter.h"
#include "Config/Setting/Setting.h"
#include <QPushButton>
#include <QLabel>
namespace MaskConfig {
BindingWidget::BindingWidget(PolygonsBinding *binding, QWidget *parent) : QWidget(parent),binding_(binding)
{

    QLabel* label = new QLabel(this);
    label->setText(binding_->ui_name());
    index_label_ = new QLabel(this);

//    prev_button_ = new QPushButton(tr("Prev"),this);
//    connect(prev_button_,SIGNAL(clicked(bool)),this,SLOT(slotPrev()));

    next_button_ = new QPushButton(tr("Next"),this);
    connect(next_button_,SIGNAL(clicked(bool)),this,SLOT(slotNext()));

   label->move(5,5);

   QRect rect(100,0,50,25);
   index_label_->setGeometry(rect);
//   prev_button_->setGeometry(rect);
   next_button_->setGeometry(rect.x()+rect.width()+5,rect.y(),rect.width(),rect.height());


   connect(binding_->getSetter()->setting(),SIGNAL(signalUpdate()),this,SLOT(updateLabel()));
}

void BindingWidget::slotNext()
{
    binding_->next();
}

void BindingWidget::slotPrev()
{
    binding_->prev();
}

void BindingWidget::updateLabel()
{
    int id = binding_->getPolyId();
    if(id < 0)
    {
        index_label_->setText(tr("None"));
    }
    else
    {
        index_label_->setText(QString::number(id));
    }
}

}
