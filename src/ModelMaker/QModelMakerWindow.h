#ifndef QMODELMAKERWINDOW_H
#define QMODELMAKERWINDOW_H

#include <QMainWindow>

namespace GCL {
class QModelMakerWidget;
class QModelMakerWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit QModelMakerWindow(QWidget *parent = nullptr);

signals:

public slots:
    void slotSetMode(bool t);
protected:
    void keyPressEvent(QKeyEvent *event);
private:
    QModelMakerWidget *maker_widget_{nullptr};
    QAction *mode_action_{nullptr};
    QAction *show_action_{nullptr};
};
}
#endif // QMODELMAKERWINDOW_H
