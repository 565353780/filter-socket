#ifndef PAINTWIDGET_H
#define PAINTWIDGET_H
#include <QWidget>
namespace MaskConfig{
class ConfigWidget;
/**
 * @brief 图片显示界面
 */
class PaintWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PaintWidget(ConfigWidget* parent);

protected:
    void resizeEvent(QResizeEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void wheelEvent(QWheelEvent *event);

    void paintEvent(QPaintEvent *event);
private:
    ConfigWidget* config_widget_;
};
}
#endif // PAINTWIDGET_H
