#ifndef CAMERAMASKWIDGET_H
#define CAMERAMASKWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QImage>
#include <QVector>
#include <QDebug>
#include <cmath>
#include <Eigen/Eigen>
#include <Eigen/Dense>
#include <Eigen/LU>
#include <iostream>

using namespace Eigen;

class CameraMaskWidget : public QWidget
{
    Q_OBJECT

public:
    CameraMaskWidget(QWidget *parent = 0);
    ~CameraMaskWidget();

    void set_background(QImage img);

    QPoint trans_widget_to_img(QPoint &widget_point);

    QPoint trans_img_to_widget(QPoint &img_point);

    QPoint trans_widget_to_draw_img(QPoint &widget_point);

    void paintEvent(QPaintEvent *event);

    //void timerEvent(QTimerEvent *event);

    void mousePressEvent(QMouseEvent *event);

    void mouseMoveEvent(QMouseEvent *event);

    void mouseReleaseEvent(QMouseEvent *event);

    void keyPressEvent(QKeyEvent *event);

    void keyReleaseEvent(QKeyEvent *event);

    void init_camera();

    void get_Draw_Background_BBox(int& x_min, int& x_max, int& y_min, int& y_max);

    void get_Common_Int(int& a, int& b, int& common_int);

    void cut_Draw_Background_By_Common_Int();

    void cut_Draw_Background_By_Fixed_Dist(int fixed_dist);

    void add_Detect_Position(int x, int y);
    void add_Detect_Position(QPoint point);

public:
    bool init_ok;

    bool show_mask;
    bool choose_point_pair;

    MatrixXd trans_matrix;

private:
    QPoint p1;
    QPoint p2;

    QPoint pair1;
    QPoint pair2;

    QImage background;
    double background_scale;
    int new_img_width;
    int new_img_height;
    QPoint start_point;

    bool remove_mode;
    bool polygon_start;
    bool mask_changed;
    bool draw_strict_line;

    QVector<QVector<QPoint>> mask_episode_list;
    QVector<QLine> point_pair_line;

    int fixed_sub_bbox_dist;
    QPoint start_bbox_point;
    MatrixXd bbox_matrix;

    bool test_mode;
};

#endif // CAMERAMASKWIDGET_H
