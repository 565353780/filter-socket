#ifndef INFOWIDGET_H
#define INFOWIDGET_H

#include <QWidget>
namespace MaskConfig {
class Setter;
/**
 * @brief //以列表形式显示标定框Id的widget
 */
class InfoWidget : public QWidget
{
    Q_OBJECT
public:
    explicit InfoWidget(Setter * setter, QWidget *parent = 0);

signals:

public slots:
    void resetGeometry();
protected:
    void mouseReleaseEvent(QMouseEvent *e);

    void paintEvent(QPaintEvent *event);


private:
    int lineHeight_;
    Setter* setter_;


};
}
#endif // INFOWIDGET_H
