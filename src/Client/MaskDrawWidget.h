#ifndef MaskDrawWidget_H
#define MaskDrawWidget_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QImage>
#include <QVector>
#include <QDebug>
#include <QDir>
#include <QJsonDocument>
#include <QByteArray>
#include <QJsonObject>
#include <cmath>

using namespace std;

class MaskDrawWidget : public QWidget
{
    Q_OBJECT

public:
    MaskDrawWidget(QWidget *parent = 0);
    ~MaskDrawWidget();

    void set_background(QImage img);

    void set_background(QString img_path);

    void update_background();

    QPoint trans_widget_to_img(QPoint &widget_point);

    void paintEvent(QPaintEvent *event);

    //void timerEvent(QTimerEvent *event);

    void mousePressEvent(QMouseEvent *event);

    void mouseMoveEvent(QMouseEvent *event);

    void mouseReleaseEvent(QMouseEvent *event);

    void keyPressEvent(QKeyEvent *event);

    void keyReleaseEvent(QKeyEvent *event);

    void findFile(const QString& path, vector<QString>& fileNames);

    void get_File_List(QString path);

    void show_Previous_Img();

    void show_Next_Img();

    void load_train_json();

    void update_train_json();

signals:
    void poly_changed();

private:
    bool write_json(QString json_file);
    bool read_json(QString json_file);

public:
    bool show_mask;

    int current_activate_poly;

    QVector<QVector<QPoint>> mask_episode_list;

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

    bool draw_strict_line;

    vector<QString> fileNames;
    int current_file_index;
    QString file_path_copy;

    QJsonObject train_json_obj;
    QString train_json_path;
};

#endif // MaskDrawWidget_H
