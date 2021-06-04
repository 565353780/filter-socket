#ifndef QSCENEMAKERWIDGET_H
#define QSCENEMAKERWIDGET_H
#include "Widgets/QRenderWidget.h"
namespace GCL {
class QMesh3D;
class QMaterial;
class QSceneMakerWidget : public QRenderWidget
{
public:
    QSceneMakerWidget(QWidget *parent = nullptr);

    void loadModel(const QString &filename);



    QMesh3D *getCurrentModel();


    void savePointMapAsObj(const QString &filename = "test.obj");

    void remove_model();

    void selectById(int id);

    void randomMake();


    void save_map();
protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
    void keyPressEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent *event);

    void initShader(const QString &vshader_filename, const QString &fshader_filename);
private:
    QMaterial *material_{nullptr};
    QList<QMesh3D *> model_list_;

    QImage x_image_;
    QImage y_image_;
    QImage z_image_;
    int current_id_{-1};

    QString recent_filename_;


};
}
#endif // SCENEMAKERWIDGET_H
