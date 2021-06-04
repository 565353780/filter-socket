#ifndef SETTER_H
#define SETTER_H
#include "Controller/Controller.h"
#include <QString>
#include <QStringList>
#include <QList>
#include <QSizeF>
#include <QPointF>
#include <QTransform>

namespace MaskConfig {
class Setting;
/**
 * @brief 选框基础元素
 */
class Setter : public Controller
{
public:
    struct Command
    {
        Command(const QString& command, int type): command_(command),type_(type){}
        int type_;
        QString command_;
    };
    Setter(Setting* setting);

    QString name() const;
    virtual ~Setter() {}
    virtual void setVideoSize(int w, int h);


    Setting* setting() {return setting_;}
    QList<Command> command_list() {return command_list_;}

    virtual int runCommand(const QString& command) {return 0;}
    virtual void init(int state = 1) {}
    virtual void finished() {}
    virtual int enter() {return 0;}
    virtual int leave() {return 0;}
    virtual int resize(int w, int h);
    virtual int mouseMove(int posX, int posY) {return 0;}
    virtual int mousePress(int posX,int posY, unsigned int button, unsigned int modifier = 0) { button_ = button; modifier_ = modifier; return 0;}
    virtual int mouseRelease(int posX, int posY, unsigned int button, unsigned int modifier = 0) {button_ = 0; modifier = 0; return 0;}
    virtual int keyUp(const unsigned int& key, unsigned int modifier = 0) {return 0;}
    virtual int keyDown(const unsigned int& key, unsigned int modifier = 0) {return 0;}
    virtual int wheel(const float& delta) {return 0;}

    virtual int paint(QPainter& painter) {return 0;}


    virtual void update() {}
    virtual void updateInfo() {}


    int current_id() const;

    virtual void set_current_id(int id);

    void setImageShowSize(QPointF img_start_point, QSizeF img_show_size);
    void getImageShowSize(QPointF &img_start_point, QSizeF &img_show_size, QSizeF &img_max_size);
public:
    QStringList info_list_;
protected:
    /**
     * @brief 屏幕坐标对应的原图(视频帧)坐标
     * @param 屏幕坐标
     * @return 原图坐标
     */
    QPointF VideoPosFromScreenPos(QPointF p);
    /**
     * @brief 原图(视频帧)坐标对应的屏幕坐标
     * @param 原图坐标
     * @return 屏幕坐标
     */
    QPointF ScreenPosFromVideoPos(QPointF p);

    virtual void paintStart(QPainter& painter);
    virtual void paintEnd(QPainter& painter);

    void updateTrans();

protected:
    int current_id_;

    QSizeF v_size_;
    QSizeF i_size_;
    Setting* setting_;
    QList<Command> command_list_;

    QPointF current_mouse_point_;
    QPointF img_start_point_;
    QSizeF img_show_size_;
    QSizeF img_max_size_;

    bool is_scale_img_;
public:
    QTransform trans;
};
}
#endif // SETTER_H
