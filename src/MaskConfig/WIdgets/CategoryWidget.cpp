#include "CategoryWidget.h"
#include "SettingRowWidget.h"
namespace MaskConfig {
CategoryWidget::CategoryWidget(const QString &category, QList<Setting *> settings, QWidget *parent):QWidget(parent),category_(category)
{
    foreach (Setting* setting, settings) {
        widgets_.push_back(new SettingRowWidget(setting,this));
    }
    int y = 15;
    int yh = 10;
    foreach (QWidget* w, widgets_) {
        w->move(0,y);
        y += w->height()+yh;
    }
    this->resize(250, y);

}
}
