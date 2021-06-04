#ifndef QPALLETIZERWIDGET_H
#define QPALLETIZERWIDGET_H

#include <QWidget>
#include <memory>
#include "Widgets/QRenderWidget.h"
#include "QueryBox.h"

#include <QKeyEvent>

class Palletizer;
namespace GCL {
class QMesh3D;
class QQuadMesh;
class QPalletizerWidget : public QRenderWidget
{
    Q_OBJECT
public:
    explicit QPalletizerWidget(QWidget *parent = nullptr);
    void initQuery(int w, int h);
signals:

public slots:
    void slotTryOnce();

    void slotLoadTestQueue(QString filename = "");
protected:
    void initializeGL();
    void paintGL();

    void keyPressEvent(QKeyEvent *event);

private:
    void randomQueryOrder();
private:
    std::shared_ptr<Palletizer> palletizer_;
    QMesh3D *cube_{nullptr};

    QQuadMesh *quad_{nullptr};
private:
    std::vector<QueryBox> query_box_list_;
    std::vector<QueryBox> online_box_list_;
    int query_count_{0};
    int target_id_{-1};

};
}
#endif // QPALLETIZERWIDGET_H
