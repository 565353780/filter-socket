#include "MaskBBoxInfoWidget.h"
#include "ui_maskbboxinfowidget.h"

MaskBBoxInfoWidget::MaskBBoxInfoWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MaskBBoxInfoWidget)
{
    ui->setupUi(this);
}

MaskBBoxInfoWidget::~MaskBBoxInfoWidget()
{
    delete ui;
}
