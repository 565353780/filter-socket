#ifndef MASKBBOXINFOWIDGET_H
#define MASKBBOXINFOWIDGET_H

#include <QWidget>
#include <QString>
#include <QDebug>
#include <iostream>

using namespace std;

namespace Ui {
class MaskBBoxInfoWidget;
}

class MaskBBoxInfoWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MaskBBoxInfoWidget(QWidget *parent = nullptr);
    ~MaskBBoxInfoWidget();

signals:
    void poly_changed();

private:
    Ui::MaskBBoxInfoWidget *ui;


};

#endif // MASKBBOXINFOWIDGET_H
