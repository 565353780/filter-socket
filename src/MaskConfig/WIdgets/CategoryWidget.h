#ifndef CATEGORYWIDGET_H
#define CATEGORYWIDGET_H
#include <QWidget>

namespace MaskConfig
{
class Setting;
class CategoryWidget : public QWidget
{
public:
    explicit CategoryWidget(const QString& category,  QList<Setting *> settings,QWidget* parent);

private:
    QList< QWidget* > widgets_;
    QString category_;

};
}

#endif // CATEGORYWIDGET_H
