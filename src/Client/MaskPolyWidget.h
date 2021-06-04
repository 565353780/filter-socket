#ifndef MASKPOLYWIDGET_H
#define MASKPOLYWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QImage>
#include <QVector>
#include <QDebug>
#include <QFileDialog>
#include <QGroupBox>
#include <cmath>

#include "MaskDrawWidget.h"

namespace Ui {
class MaskPolyWidget;
}

class MaskPolyWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MaskPolyWidget(QWidget *parent = nullptr);
    ~MaskPolyWidget();

private slots:
    void on_Btn_OpenDir_clicked();

    void on_Btn_PreImg_clicked();

    void on_Btn_NextImg_clicked();

    void on_Btn_EditLabel_clicked();

    void on_MaskDrawWidget_Poly_Changed();

    void on_cBox_Label_currentIndexChanged(int index);

    void on_Btn_Label_Pressed();

private:
    Ui::MaskPolyWidget *ui;

    QString labels_string_copy;
    QStringList label_list;

    QVector<int> label_index_list;

    MaskDrawWidget* mask_draw_widget;

    QVector<QPushButton*> btn_list;
    QGroupBox* empty_group;
};

#endif // MASKPOLYWIDGET_H
