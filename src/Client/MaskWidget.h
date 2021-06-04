#ifndef MASKWIDGET_H
#define MASKWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QImage>
#include <QVector>
#include <QDebug>
#include <cmath>

class MaskWidget : public QWidget
{
    Q_OBJECT

public:
    MaskWidget(QWidget *parent = 0);
    ~MaskWidget();

    void set_background(QImage img);

    QPoint trans_widget_to_img(QPoint &widget_point);

    void paintEvent(QPaintEvent *event);

    //void timerEvent(QTimerEvent *event);

    void mousePressEvent(QMouseEvent *event);

    void mouseMoveEvent(QMouseEvent *event);

    void mouseReleaseEvent(QMouseEvent *event);

    void keyPressEvent(QKeyEvent *event);

public:
    bool show_mask;

private:
    QPoint p1;
    QPoint p2;

    QImage background;
    double background_scale;
    int new_img_width;
    int new_img_height;
    QPoint start_point;

    bool remove_mode;
    bool polygon_start;
    bool mask_changed;

    QVector<QVector<QPoint>> mask_episode_list;
};

#endif // MASKWIDGET_H
