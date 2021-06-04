#include "PanelTabDialog.h"
#include "Config/Config.h"
#include "WIdgets/ConfigWidget.h"
#include <QTabWidget>
#include <QLabel>
#include "SetterWidget.h"
#include <QKeyEvent>
#include <QMessageBox>
#include "Config/Setter/Setter.h"
#include "Config/Setting/Setting.h"
#include "CategoryWidget.h"

#include <QDebug>
namespace MaskConfig {
PanelTabDialog::PanelTabDialog(ConfigWidget *parent) : QWidget(parent),config_widget_(parent)
{
    tab_widget_ = new QTabWidget(this);

    QList<QString> keys = config_widget_->config()->keys();
    QMap<QString,QList< Setting* > > category_settings;
    foreach (QString key, keys) {
        Setting* setting = config_widget_->config()->getSetting(key);
        if(setting && !setting->has_setter())
        {
            category_settings[setting->category()].push_back(setting);
        }
    }



    foreach (QString key, keys) {
        Setter* setter = config_widget_->getSetter(key);

        if(setter)
        {
            setter_widget_ = new SetterWidget(key, setter,tab_widget_);
            connect(setter_widget_,SIGNAL(signalUpdate()),config_widget_,SLOT(update()));
            connect(setter_widget_,&SetterWidget::signalUpdate,config_widget_,&ConfigWidget::slotUpdatePaint);
            tab_widget_->addTab(setter_widget_,config_widget_->config()->getName(key));
        }

    }

    QList<QString> category_list = category_settings.keys();
    foreach (const QString& category, category_list)
    {
        qDebug()<<category<<"... New";

        CategoryWidget* w = new CategoryWidget(category, category_settings[category],tab_widget_);
        tab_widget_->addTab(w,category);

        qDebug()<<category<<"... New Done";

    }
    connect(tab_widget_, SIGNAL(currentChanged(int)),this,SLOT(slotTabChanged(int)));
}

void PanelTabDialog::slotTabChanged(int index)
{
    QWidget* w = tab_widget_->widget(index);
    SetterWidget* widget = dynamic_cast<SetterWidget * >(w);
    if(widget)
    {
        config_widget_->changeSetter(widget->key());
    }
}

void PanelTabDialog::resizeEvent(QResizeEvent *)
{
    tab_widget_->setGeometry(5,5,this->width()-10,this->height()-10);
    int y = tab_widget_->y() + tab_widget_->height() + 5;
}

void PanelTabDialog::keyPressEvent(QKeyEvent *e)
{
    if(config_widget_->current_setter())
    {
        config_widget_->current_setter()->keyDown(e->key(),e->modifiers());
        config_widget_->update();
    }
}
}
