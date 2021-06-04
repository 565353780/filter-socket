#ifndef TRACKIDWIDGET_H
#define TRACKIDWIDGET_H

#include <QWidget>
class QLineEdit;
class QLabel;
namespace MaskConfig {
class BBoxSetter;
class BBoxSetting;
class TrackIdWidget : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief BBox追踪
     * @param bbox_setter
     * @param parent
     */
    explicit TrackIdWidget(BBoxSetter *bbox_setter, QWidget *parent = 0);

signals:

public slots:
    void slotNextIdChanged(const QString& str);
    void slotIdChanged(const QString& str);

    void slotUpdate(int t);

protected:
    void setNextId(int id);

    void resetGeometry();

    BBoxSetting *get_setting();
private:
    int id_;
    int next_id_;
    BBoxSetter *bbox_setter_;

private:
    QLabel *next_id_label_;
    QLabel *id_label_;
    QLineEdit* next_id_edit_;
    QLineEdit* id_edit_;
};
}
#endif // TRACKIDWIDGET_H
