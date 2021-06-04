#ifndef QMODELMAKERWIDGET_H
#define QMODELMAKERWIDGET_H
#include "Widgets/QRenderWidget.h"
#include <QVector3D>
#include <QJsonObject>
namespace GCL
{
class QMesh3D;
class QMaterial;
class QQuadMesh;
class QPrimitiveMesh;
class QAxesMesh;
class QInputTransformDialog;
class QModelMakerWidget : public QRenderWidget
{
    Q_OBJECT
public:
    enum VertexStatusType
    {
        Invalid  = 0,
        Valid = 1,
        Selected = 2,
        Deleted = 3
    };

    enum TransformMode
    {
        TM_None = 0,
        TM_TranslateModel = 1,
        TM_RotateModel = 2,
        TM_TranslatePickpoint = 3,
        TM_RotatePickpoint = 4
    };

    enum SelectMode
    {
        Selector = 0,
        Eraser = 1,
        Repairer = 2,
        PickPoint = 3
    };
    struct VertexAttachedInfo
    {
        VertexAttachedInfo(int mid = -1, int fid = -1, int status = Invalid): mid_(mid), fid_{fid}, status_{status} {}
        void setStatus(int t)
        {
            status_ = t;
        }
        void fromColor(const QVector4D &color);
        QVector4D toColor(bool *ok = nullptr) const;

        int mid_{-1};
        int fid_{-1};
        int status_{0};
    };

    struct PickPointInfo
    {
        QJsonObject toJson() const;
        QJsonObject toSimpleJson() const;
        QString model_name_;
        int id_{-1};
        int mid_{-1};
        int fid_{-1};
        QVector3D offset_;
        QVector3D euler_;
        QVector3D mesh_offset_;
        QVector3D mesh_euler_;
        QVector3D place_offset_;
        QVector3D place_euler_;
    };



    explicit QModelMakerWidget(QWidget *parent = nullptr);

    void loadModel(const QString &filename);

    void selectById(int id);

    QMesh3D *getCurrentModel();
    QMesh3D *getModelAt(int i);

    void saveMeshObj(const QString &filename, QMesh3D *mesh);
    void saveObj(const QString &filename);
    void setSelectMode(int mode);
    void cutByZPlane(float zval);

    void selectByRect(QRectF rect, int mode);

    void removePickPoint();
    void removeCurrentModel();

    void savePickPoints(const QString &filename);

public slots:
    void slotLoadModel();
    void slotSaveModel();
    void slotChangeMode(bool t);

    void slotSaveResults();

    void slotRecordPlaceTransform();
    void slotShowPlaceTransform();
    void slotOriginTransform();
    void slotDialogChanged(QVector3D offset, QVector3D euler);
    void saveFeaturePoints();



    void slotUpdateVertices();
    void slotMoveModelCenter();
    void deleteUnselection();
    void slotCutByZval();


    void slotShowAll();

    void slotShowOnlySeclected();

    void slotRotateBySelectedFaces();
public:
    void keyPressEvent(QKeyEvent *event);



protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *e);
    void initShader(const QString &vshader_filename, const QString &fshader_filename);


protected:
    void translate(QVector3D offset);
    void rotate(QVector3D euler);
    void applyLeftEuler(QVector3D euler);
    PickPointInfo *getCurrentPickInfo();
protected:
    void saveBuffer();
    void pickAt(QPoint pos, bool isMoving = false);

    QPoint rect_p0_;
    QPoint rect_p1_;
private:
    QMaterial *material_{nullptr};
    QMap<int, QMesh3D *> model_list_;
    QQuadMesh *rect_mesh_{nullptr};
    QImage info_image_;


    int current_id_{-1};


    int mode_{Selector};
    QAxesMesh *axes_{nullptr};
    QPrimitiveMesh *ball_{nullptr};
    QString recent_filename_;

    QList<PickPointInfo> pickpoint_list_;
    int select_pick_id_{-1};

    QMesh3D *plane_{nullptr};


private:
    QInputTransformDialog *transform_dialog_{nullptr};
    int v3_dlg_type_{TM_None};
    bool is_show_selected_only_{false};

};
}
#endif // QMODELMAKERWIDGET_H
