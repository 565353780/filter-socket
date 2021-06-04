#include "QModelMakerWidget.h"
#include "Core/Q3DScene.h"
#include "Core/QMesh3D.h"
#include "Core/QMaterial.h"
#include "Core/QMathUtil.h"
#include "Meshes/QQuadMesh.h"
#include <QDebug>
#include <QFileDialog>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QOpenGLFramebufferObject>
#include "Core/QMeshParser.h"
#include <QInputDialog>
#include <ctime>
#include <QPainter>
#include <QFileInfo>
#include "Meshes/QQuadMesh.h"
#include "Meshes/QPrimitiveMesh.h"
#include "Core/Math/MathDefines.h"
#include "Meshes/QAxesMesh.h"
#include "Core/QMeshGroup.h"
#include <QMessageBox>
#include "Widgets/QInputTransformDialog.h"
#include "Finder/QFinderConfig.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QRegularExpression>
#include <QProcess>

namespace GCL
{

QModelMakerWidget::QModelMakerWidget(QWidget *parent): QRenderWidget(parent)
{
    scene_->setRotationSpeed(2);
    scene_->setPanSpeed(40);
    scene_->setScaleSpeed(1);
    scene_->setPanSpeed(20);
    scene_->setProjectionParameters(0.1, 10000.0, 45);

    transform_dialog_ = new QInputTransformDialog(this);

    connect(transform_dialog_, SIGNAL(signalValueChanged(QVector3D, QVector3D)), this, SLOT(slotDialogChanged(QVector3D, QVector3D)));
    this->setFocusPolicy(Qt::StrongFocus);


}

void QModelMakerWidget::loadModel(const QString &filename)
{
    makeCurrent();
    QMesh3D *mesh = nullptr;


    std::vector<QMesh3D::VertexData> array_data;
    std::vector<QMesh3D::FaceData> face_data;
    QTriMeshParser::loadFile(filename, array_data, face_data, true);

    if(array_data.size() == 0) return;
    for(int i = 0; i < array_data.size() / 3; i++)
    {
        VertexAttachedInfo vinfo(model_list_.size(), i, Valid);

        for(int j = 0; j < 3; j++)
        {
            float dz = array_data[3 * i + (j + 1) % 3].position_[2] - array_data[3 * i + (j) % 3].position_[2];
            dz = abs(dz);
//            if(dz > 10) vinfo.status_ = Invalid;

        }

        QVector4D color = vinfo.toColor();
        array_data[3 * i].color_ = color;
        array_data[3 * i + 1].color_ = color;
        array_data[3 * i + 2].color_ = color;
    }
    if(material_)
    {
        mesh = new QMesh3D(material_, scene_);
    }
    else
    {
        mesh = new QMesh3D(scene_);
    }
    QFileInfo fileinfo(filename);

    mesh->setObjectName(fileinfo.baseName());



    mesh->setHasSavedata(true);
    mesh->updateArrayBuffer(array_data);
    mesh->updateIndexBuffer(face_data);


    recent_filename_ = filename;
    if(getCurrentModel())
    {
        getCurrentModel()->setSelected(false);
    }
    current_id_ = this->model_list_.size();
    this->model_list_[current_id_] = (mesh);
    mesh->setColor(QVector3D(0.6, 0.6, 0.6));
    this->selectById(current_id_);
    this->saveBuffer();
    update();


}

void QModelMakerWidget::selectById(int id)
{
    if(getCurrentModel())
    {
        getCurrentModel()->setSelected(false);
    }
    if(model_list_.size() == 0) return;
    if(id < 0)
    {
        id = model_list_.size() - 1;
    }
    else if(id >= model_list_.size())
    {
        id %= model_list_.size();
    }

    while (id >= 0 && model_list_.count(id) == 0)
    {
        id--;
    }
    if(id < 0)
    {
        current_id_ = -1;
        return;
    }
    current_id_ = id;
//    transform_dialog_->hide();
    QMesh3D *mesh = getCurrentModel();

    this->translate(QVector3D(0, 0, 0));
    if(mesh)
    {
        mesh->setSelected(true);
        this->setWindowTitle(mesh->objectName());
    }

    if(is_show_selected_only_)
    {
        slotShowOnlySeclected();
    }
    else
    {
        slotShowAll();
    }



}

QMesh3D *QModelMakerWidget::getCurrentModel()
{
    if(current_id_ >= 0 && model_list_.count(current_id_))
    {
        return model_list_[current_id_];
    }
    else
    {
        return nullptr;
    }
}

QMesh3D *QModelMakerWidget::getModelAt(int i)
{
    if(i < 0 || i >= model_list_.size() || !model_list_.count(i))
    {
        return nullptr;
    }
    return model_list_[i];
}

void QModelMakerWidget::saveMeshObj(const QString &filename, QMesh3D *mesh)
{
    makeCurrent();
    FILE *fp = fopen(filename.toLocal8Bit().data(), "w");
    QMap<Vec3, int> vmap;
    std::vector< QMesh3D::VertexData> vertices;
    mesh->readVertices(vertices);
    for(int i = 0; i < vertices.size() / 3; i++)
    {
        QVector3D v[3];
        int status = Valid;
        for(int j = 0; j < 3; j++)
        {
            VertexAttachedInfo vinfo;
            vinfo.fromColor(vertices[3 * i + j].color_);
            if(vinfo.status_ == Deleted)
            {
                status = Deleted;
                break;
            }
        }
        if(status == Deleted) continue;
        int f[3];
        for(int j = 0; j < 3; j++)
        {
            v[j] = (vertices[3 * i + j].position_);
            Vec3 vv(v[j].x(), v[j].y(), v[j].z());

            if(!vmap.count(vv))
            {
                int tcount = vmap.size();
                vmap[vv] = tcount;
                fprintf(fp, "v %f %f %f\n", v[j].x(), v[j].y(), v[j].z());
            }
            f[j] = vmap[vv] + 1;
        }
        fprintf(fp, "f %d %d %d\n", f[0], f[1], f[2]);
    }
    fclose(fp);
}
void QModelMakerWidget::saveObj(const QString &filename)
{

    int only_selected_face = QMessageBox::question(0, tr("Save Obj"), tr("Only Selected Faces?"),
                             tr("OnlySelectedFaces"), tr("OnlySelectedModel"), tr("All"));

    FILE *fp = fopen(filename.toLocal8Bit().data(), "w");
    QMap<Vec3, int> vmap;
    for(QMesh3D *mesh : model_list_)
    {
        std::vector< QMesh3D::VertexData> vertices;
        mesh->readVertices(vertices);
        if(only_selected_face == 1)
        {
            if(!mesh->isSelected()) continue;
        }
        for(int i = 0; i < vertices.size() / 3; i++)
        {
            QVector3D v[3];
            int status = Valid;
            for(int j = 0; j < 3; j++)
            {
                VertexAttachedInfo vinfo;
                vinfo.fromColor(vertices[3 * i + j].color_);
                if(vinfo.status_ == Deleted)
                {
                    status = Deleted;
                    break;
                }
            }
            if(status == Deleted) continue;

            if(only_selected_face == 0)
            {
                int scount = 0;
                for(int j = 0; j < 3; j++)
                {
                    VertexAttachedInfo vinfo;
                    vinfo.fromColor(vertices[3 * i + j].color_);
                    if(vinfo.status_ == Selected)
                    {
                        scount++;

                    }
                }
                if(scount != 3)
                {
                    continue;
                }
            }




            int f[3];
            for(int j = 0; j < 3; j++)
            {
                v[j] = mesh->localTransform(vertices[3 * i + j].position_);
                Vec3 vv(v[j].x(), v[j].y(), v[j].z());

                if(!vmap.count(vv))
                {
                    int vcount = vmap.size();
                    vmap[vv] = vcount;
                    fprintf(fp, "v %f %f %f\n", v[j].x(), v[j].y(), v[j].z());
                }
                f[j] = vmap[vv] + 1;
            }
            fprintf(fp, "f %d %d %d\n", f[0], f[1], f[2]);
        }
    }

    fclose(fp);

}

void QModelMakerWidget::setSelectMode(int mode)
{
    mode_ = mode;
    if(mode == Selector)
    {
        this->setCursor(Qt::ArrowCursor);
    }
    else if(mode_ == Eraser)
    {
        QPixmap pixmap(":/eraser");
        this->setCursor(QCursor(pixmap.scaled(32, 32, Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));
    }
    else if(mode_ == Repairer)
    {
        QPixmap pixmap(":/circle");
        this->setCursor(QCursor(pixmap.scaled(32, 32, Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));
    }
}

void QModelMakerWidget::cutByZPlane(float zval)
{
    QMesh3D* mesh = getCurrentModel();
    if(!mesh) return;
    makeCurrent();
    std::vector<QMesh3D::VertexData> data;
    mesh->readVertices(data);
    for(int i = 0; i < data.size() / 3; i++)
    {
        QMesh3D::VertexData v[3];
        for(int j = 0; j < 3; j++)
        {
            v[j] = data[3 * i + j];
        }
        int ttype = 0;
        for(int j = 0; j < 3; j++)
        {
            QVector3D pos = mesh->localTransform(v[j].position_);
            if(pos.z() < zval)
            {
                ttype = Deleted;
            }
        }

        if(ttype == Deleted)
        {
            for(int j = 0; j < 3; j++)
            {
                VertexAttachedInfo vinfo;
                vinfo.fromColor(v[j].color_);
                vinfo.status_ = Deleted;
                v[j].color_ = vinfo.toColor();
                data[3 * i + j] = v[j];
            }

        }
        else
        {
            bool flag = false;
            for(int j = 0; j < 3; j++)
            {
                VertexAttachedInfo vinfo;
                vinfo.fromColor(v[j].color_);
                if(vinfo.status_ == Deleted)
                {
                    flag = true;
                }
            }
            if(flag)
            {
                for(int j = 0; j < 3; j++)
                {
                    VertexAttachedInfo vinfo;
                    vinfo.fromColor(v[j].color_);
                    vinfo.status_ = Valid;
                    v[j].color_ = vinfo.toColor();
                    data[3 * i + j] = v[j];
                }
            }
        }

    }
    mesh->updateArrayBuffer(data);


}

void QModelMakerWidget::selectByRect(QRectF rect, int mode)
{
    QMesh3D *mesh = getCurrentModel();
    if(!mesh) return;



    std::vector<QMesh3D::VertexData> vertices;
    makeCurrent();
    mesh->readVertices(vertices);

    for(int i = 0; i < vertices.size(); i++)
    {
        QVector3D v = vertices[i].position_;
        v = mesh->localTransform(v);
        v = scene_->project(v);
        QPointF p(v.x() / v.z(), v.y() / v.z());
        if(!rect.contains(p)) continue;
        VertexAttachedInfo vinfo;
        vinfo.fromColor(vertices[i].color_);
        if(vinfo.status_ == Invalid) continue;
        if(vinfo.status_ != Deleted)
        {
            if(mode == Selector)
            {
                vinfo.status_ = Selected;

            }
            else if(mode == Eraser)
            {
                vinfo.status_ = Valid;
            }
        }
        else if(mode == Repairer)
        {
            vinfo.status_ = Valid;
        }

        vertices[i].color_ = vinfo.toColor();
    }
    mesh->updateArrayBuffer(vertices);
}

void QModelMakerWidget::deleteUnselection()
{
    QMesh3D *mesh = getCurrentModel();
    if(!mesh) return;

    std::vector< QMesh3D::VertexData > vdata;

    mesh->readVertices(vdata);

    for(auto &v : vdata)
    {
        VertexAttachedInfo vinfo;
        vinfo.fromColor(v.color_);
        if(vinfo.status_ != Selected)
        {
            vinfo.status_ = Deleted;
        }

        v.color_ = vinfo.toColor();
    }
    makeCurrent();
    mesh->writeVertices(vdata);


}

void QModelMakerWidget::slotCutByZval()
{
    bool ok = false;
    double zval = QInputDialog::getDouble(0, tr(""), tr("Z Val"), -1000, -1000, 2000, 1, &ok);
    if(ok)
    {
        cutByZPlane(zval);
        update();
    }
}

void QModelMakerWidget::slotShowAll()
{
    is_show_selected_only_ = false;
    for(QMesh3D *mesh : model_list_)
    {
        mesh->setVisible(true);
    }
    update();
}

void QModelMakerWidget::slotShowOnlySeclected()
{
    is_show_selected_only_ = true;
    for(QMesh3D *mesh : model_list_)
    {
        mesh->setVisible(mesh->isSelected());
    }
    update();
}

void QModelMakerWidget::slotRotateBySelectedFaces()
{
    QMesh3D *mesh = getCurrentModel();
    if(!mesh) return;

    makeCurrent();
    std::vector< QMesh3D::VertexData > vdata;

    mesh->readVertices(vdata);

    QList<QVector3D> vlist;
    QVector3D vcenter;
    QVector3D vnor;
    for(auto &v : vdata)
    {
        VertexAttachedInfo vinfo;
        vinfo.fromColor(v.color_);
        if(vinfo.status_ == Selected)
        {
            vnor += v.normal_;
            vcenter += v.position_;
            vlist.push_back(v.position_);
        }
    }

    vnor.normalize();
    vcenter /= vlist.size();

//    QVector3D axis[3];
//    QMathUtil::computePCA(vlist,axis[0],axis[1],axis[2]);
//    QVector3D tnor = vnor;
//    float maxval = -1;
//    for(int i=0; i<3;i++)
//    {
//        axis[i].normalize();
//        float val = tnor.dotProduct(tnor, axis[i]);
//        if(fabs(val) > maxval)
//        {
//            if(val < 0) vnor = -axis[i];
//            else vnor = axis[i];
//            maxval = fabs(val);
//        }
//    }

//    qDebug()<<tnor<<" "<<vnor;


    mesh->setEuler( QMathUtil::fromVectorTransformToEuler(vnor, QVector3D(0, 0, 1)));
    update();



}

void QModelMakerWidget::removePickPoint()
{
    if(select_pick_id_ >= 0 && select_pick_id_ < pickpoint_list_.size())
    {
        pickpoint_list_.removeAt(select_pick_id_);
    }

    for(int i = 0; i < pickpoint_list_.size(); i++)
    {
        pickpoint_list_[i].id_ = i;
    }
    if(select_pick_id_ >= pickpoint_list_.size())
    {
        select_pick_id_ = pickpoint_list_.size() - 1;
    }
}

void QModelMakerWidget::removeCurrentModel()
{
    QMesh3D *mesh = getCurrentModel();
    if(!mesh) return;
    mesh->deleteLater();
    model_list_[current_id_] = nullptr;
    model_list_.remove(current_id_);
    selectById(current_id_ - 1);
}

void QModelMakerWidget::savePickPoints(const QString &filename)
{
    QJsonObject obj;
    QMap<QString, QJsonArray> pickMap;
    for(const auto &pickpoint : pickpoint_list_)
    {
        if(!pickMap.contains(pickpoint.model_name_))
        {
            pickMap[pickpoint.model_name_] = QJsonArray();
        }

        pickMap[pickpoint.model_name_].push_back(pickpoint.toJson());
    }

    QList<QString> modelnames = pickMap.keys();

    for(QString name : modelnames)
    {
        obj[name] = QJsonObject();
        QJsonObject tobj;
        tobj["pickpoints"] = pickMap[name];
        obj[name] = tobj;
    }

    QJsonDocument doc(obj);
    QFile file(filename);

    if(!file.open(QFile::WriteOnly))
    {
        qDebug() << filename << " open failed";
        return;
    }

    QString data = doc.toJson(QJsonDocument::Indented);
    QRegularExpression re;
    re.setPattern("\\[(\\n\\ *(-\|\\d\|\\.|,)*){6}\\\n\\ *\\]");
    QRegularExpressionMatch match = re.match(data);
    int mcount = 0;
    while(match.hasMatch())
    {
        QString oldstring = match.captured(0);
        QString newstring = oldstring;
        newstring.replace(" ", "");
        newstring.replace("\n", "");
        data.replace(oldstring, newstring);
        match = re.match(data);
        mcount++;
        if(mcount > 100) break;
    }
    file.write(data.toLocal8Bit());

    file.close();

}

void QModelMakerWidget::saveFeaturePoints()
{
    QString filename = QFileDialog::getSaveFileName(nullptr, "", "", "*.json");
    if(filename.isEmpty()) return;
    QJsonObject obj;
    QMap<QString, QJsonArray> pickMap;
    for(const auto &pickpoint : pickpoint_list_)
    {
        if(!pickMap.contains(pickpoint.model_name_))
        {
            pickMap[pickpoint.model_name_] = QJsonArray();
        }

        pickMap[pickpoint.model_name_].push_back(pickpoint.toSimpleJson());
    }

    QList<QString> modelnames = pickMap.keys();

    for(QString name : modelnames)
    {
        obj[name] = QJsonObject();
        QJsonObject tobj;
        tobj["featurepoints"] = pickMap[name];
        obj[name] = tobj;
    }

    QJsonDocument doc(obj);
    QFile file(filename);

    if(!file.open(QFile::WriteOnly))
    {
        qDebug() << filename << " open failed";
        return;
    }

    QString data = doc.toJson(QJsonDocument::Indented);
    QRegularExpression re;
    re.setPattern("\\[(\\n\\ *(-\|\\d\|\\.|,)*){6}\\\n\\ *\\]");
    QRegularExpressionMatch match = re.match(data);
    int mcount = 0;
    while(match.hasMatch())
    {
        QString oldstring = match.captured(0);
        QString newstring = oldstring;
        newstring.replace(" ", "");
        newstring.replace("\n", "");
        data.replace(oldstring, newstring);
        match = re.match(data);
        mcount++;
        if(mcount > 100) break;
    }
    file.write(data.toLocal8Bit());

    file.close();
}

void QModelMakerWidget::slotLoadModel()
{
    QStringList list = QFileDialog::getOpenFileNames(nullptr, tr(""), "./", "*.obj *.ply");
    if(list.isEmpty()) return;

    for(auto filename : list)
    {
        this->loadModel(filename);
    }
}

void QModelMakerWidget::slotSaveModel()
{
    if(mode_ == PickPoint)
    {
        QString filename = QFileDialog::getSaveFileName(0, tr(""), "./", "*.json");
        if(filename.isEmpty()) return;
        savePickPoints(filename);
    }
    else
    {
        QString filename = QFileDialog::getSaveFileName(0, tr(""), "./", "*.obj");
        if(filename.isEmpty()) return;
        this->saveObj(filename);
    }

}

void QModelMakerWidget::slotChangeMode(bool t)
{
    if(!t)
    {
        mode_ = Selector;
        this->setCursor(Qt::ArrowCursor);
    }
    else
    {
        mode_ = PickPoint;
        this->setCursor(Qt::PointingHandCursor);
    }
}

void QModelMakerWidget::slotSaveResults()
{
    QString filename = QFileDialog::getSaveFileName(0, "", "", "*.json");
    if(filename.isEmpty()) return;
    savePickPoints(filename);

    QFileInfo fileinfo(filename);


    for(int i = 0; i < model_list_.size(); i++)
    {
        if(!model_list_.count(i)) continue;
        if(!model_list_[i]) continue;

        QString obj_filename = fileinfo.dir().absoluteFilePath(model_list_[i]->objectName() + ".obj");
        saveMeshObj(obj_filename, model_list_[i]);


        QString command = ".\\MeshLab\\meshlabserver_32.exe -i \"" + obj_filename + "\" -o \"" + obj_filename + "\" -s .\\MeshLab\\mesh_simplify.mlx";
        QProcess process;
        process.start(command);
        process.waitForFinished(3000);

        qDebug() << process.readAll();
    }

}

void QModelMakerWidget::slotRecordPlaceTransform()
{
    PickPointInfo *pickinfo = getCurrentPickInfo();
    if(!pickinfo) return;
    QMesh3D *mesh = getModelAt( pickinfo->mid_) ;
    if(!mesh) return;
    QVector3D p = pickinfo->offset_;
    p = mesh->localTransform(p);
    axes_->setEuler(pickinfo->euler_);
    axes_->setOffset(p);
    axes_->applyLeftEuler(mesh->getEuler());

    pickinfo->mesh_euler_ = mesh->getEuler();
    pickinfo->mesh_offset_ = mesh->getOffset();
    pickinfo->place_euler_ = axes_->getEuler();
    pickinfo->place_offset_ = axes_->getOffset();
}

void QModelMakerWidget::slotShowPlaceTransform()
{
    PickPointInfo *pickinfo = getCurrentPickInfo();
    if(!pickinfo) return;
    QMesh3D *mesh = getModelAt( pickinfo->mid_);
    if(!mesh) return;

    mesh->setOffset(pickinfo->mesh_offset_);
    mesh->setEuler(pickinfo->mesh_euler_);
    update();

}

void QModelMakerWidget::slotOriginTransform()
{
    QMesh3D *mesh = getCurrentModel();
    if(!mesh) return;

    mesh->setOffset(QVector3D(0, 0, 0));
    mesh->setEuler(QVector3D(0, 0, 0));

    update();
}

void QModelMakerWidget::slotDialogChanged(QVector3D offset, QVector3D euler)
{
    if(mode_ == PickPoint)
    {
        PickPointInfo *pickinfo = getCurrentPickInfo();
        if(!pickinfo) return;
        if(transform_dialog_->isHidden()) return;

        pickinfo->offset_ = offset;
        pickinfo->euler_ = euler;


    }
    else
    {
        QMesh3D *mesh = getCurrentModel();
        if(!mesh) return;
        if(transform_dialog_->isHidden()) return;


        mesh->setOffset(offset);
        mesh->setEuler(euler);


    }

    update();
}

void QModelMakerWidget::slotUpdateVertices()
{
    makeCurrent();
    QMesh3D *mesh = getCurrentModel();
    if(!mesh) return;
    std::vector<QMesh3D::VertexData> vdata;
    mesh->readVertices(vdata);
    for(auto &v : vdata)
    {
        v.position_ = mesh->localTransform(v.position_);
        v.normal_ = mesh->localRotate(v.normal_);
    }
    mesh->setOffset(QVector3D(0, 0, 0));
    mesh->setEuler(QVector3D(0, 0, 0));
    mesh->updateArrayBuffer(vdata);
    update();
}

void QModelMakerWidget::slotMoveModelCenter()
{
    makeCurrent();
    QMesh3D *mesh = getCurrentModel();
    if(!mesh) return;
    QVector3D offset = mesh->getOffset();
    QVector3D vmin, vmax;
    mesh->getTransformedBBox(vmin, vmax);
    mesh->setOffset(offset - (vmin + vmax) * 0.5);

    std::vector<QMesh3D::VertexData> vdata;
    mesh->readVertices(vdata);
    for(auto &v : vdata)
    {
        v.position_ = mesh->localTransform(v.position_);
        v.normal_ = mesh->localRotate(v.normal_);
    }
    mesh->setOffset(QVector3D(0, 0, 0));
    mesh->setEuler(QVector3D(0, 0, 0));
    mesh->updateArrayBuffer(vdata);
    update();
}

void QModelMakerWidget::initializeGL()
{
    QRenderWidget::initializeGL();

    this->initShader(":/shaders/modelmaker_vshader.glsl", ":/shaders/modelmaker_fshader.glsl");
    material_->addUniformValue("u_showtype", int(0));
    QMatrix4x4 matrix;
    matrix.lookAt(QVector3D(0, 0, 1), QVector3D(0, 0, 0), QVector3D(0, 1, 0));
    matrix.translate(0, 0, -299);
    scene_->setDefaultModelMatrix(matrix);
    scene_->setDefaultView();


    QQuadMesh *quad = new QQuadMesh(scene_);
    quad->setObjectName("quad");
    quad->setElementType(GL_LINES);
    quad->getMaterial()->addUniformValue("u_showtype", int(1));
    quad->setBackground(true);
    quad->setVisible(false);
    rect_mesh_ = quad;


    ball_ = new QPrimitiveMesh(QPrimitiveMesh::Sphere, scene_);

    ball_->setObjectName("BALL");
    ball_->setScale(QVector3D(0.2, 0.2, 0.2));
    ball_->setColor(QVector4D(0.9, 0.3, 0.2, 1));
    ball_->setVisible(false);
    ball_->init();


    plane_ = new QMeshGroup(scene_);
    plane_->setVisible(false);
    QQuadMesh *plane_layer0 = new GCL::QQuadMesh(plane_);
    plane_layer0->init();
    plane_layer0->setObjectName("layer0");
    plane_layer0->setScale(QVector3D(100, 100, 1));
    plane_layer0->getMaterial()->addUniformTextureImage("texture", QImage(":/images/grid.png"), QOpenGLTexture::Linear, QOpenGLTexture::Linear);
    plane_layer0->getMaterial()->addUniformValue("u_use_texture", 1);
    plane_layer0->getMaterial()->addUniformValue("u_repeat_v", QVector2D(20, 20));
    plane_layer0->getMaterial()->addUniformValue("u_blend_val", float(0.15));
    plane_layer0->setColor(QVector4D(0.2, 0.8, 0.8, 1.0));



    QQuadMesh *plane_layer1 = new GCL::QQuadMesh(plane_);
    plane_layer1->init();
    plane_layer1->setObjectName("layer1");
    plane_layer1->setScale(QVector3D(100, 100, 1));
    plane_layer1->setOffset(QVector3D(0, 0, 0.1));
    plane_layer1->getMaterial()->addUniformTextureImage("texture", QImage(":/images/grid.png"), QOpenGLTexture::Linear, QOpenGLTexture::Linear);
    plane_layer1->getMaterial()->addUniformValue("u_use_texture", 1);
    plane_layer1->getMaterial()->addUniformValue("u_repeat_v", QVector2D(1, 1));
    plane_layer1->getMaterial()->addUniformValue("u_blend_val", float(0.0));
    plane_layer1->setColor(QVector4D(0.2, 0.7, 0.7, 0.4));


    axes_ = new QAxesMesh(this);
    axes_->init();
    axes_->setRadius(1);
    axes_->setLength(5);
    axes_->setVisible(false);
}

void QModelMakerWidget::paintGL()
{
    QRenderWidget::paintGL();
    glEnable(GL_DEPTH_TEST);

    QMatrix4x4 model_matrix = scene_->getModelMatrix();
    QMatrix4x4 project_matrix = scene_->getProjectionMatrix();
    for(const auto &pick_info : pickpoint_list_)
    {
        QVector3D p = pick_info.offset_;

        QMesh3D *mesh = getModelAt(pick_info.mid_ );
        if(!mesh) continue;
        p = mesh->localTransform(p);
        if(is_show_selected_only_)
        {
            if(pick_info.mid_ != current_id_)
            {
                continue;
            }
        }

        if(pick_info.id_ == select_pick_id_)
        {
            ball_->setColor(QVector4D(1.0, 0.8, 0.2, 1.0));
            axes_->setEuler(pick_info.euler_);
            axes_->setOffset(p);

            axes_->applyLeftEuler(mesh->getEuler());
            axes_->render(project_matrix, model_matrix);
        }
        else
        {
            ball_->setColor(QVector4D(0.3, 0.3, 0.8, 1));

        }
        ball_->setOffset(p);
        ball_->render(project_matrix, model_matrix);

    }

    glEnable(GL_BLEND);
    plane_->render(project_matrix, model_matrix);
    glDisable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);

}

void QModelMakerWidget::resizeGL(int w, int h)
{
    QRenderWidget::resizeGL(w, h);

    transform_dialog_->move(this->mapToGlobal(QPoint(this->width() - transform_dialog_->width() - 10, 0)));
}

void QModelMakerWidget::keyPressEvent(QKeyEvent *event)
{
    if(event->modifiers() == Qt::ControlModifier)
    {
        if(event->key() == Qt::Key_O)
        {
            QString filename = QFileDialog::getOpenFileName(0, tr(""), "./", "*.obj");
            if(filename.isEmpty()) return;

            this->loadModel(filename);
        }
        else if(event->key() == Qt::Key_S)
        {
            QString filename = QFileDialog::getSaveFileName(0, tr(""), "./", "*.obj");
            if(filename.isEmpty()) return;
            this->saveObj(filename);

        }
        else if(event->key() == Qt::Key_I)
        {
            this->loadModel(recent_filename_);
        }
        else if(event->key() == Qt::Key_Left)
        {
            selectById(current_id_ - 1);
            update();
        }
        else if(event->key() == Qt::Key_Right)
        {
            selectById(current_id_ + 1);
            update();
        }
        else if(event->key() == Qt::Key_L)
        {
            bool ok = false;
            double zval = QInputDialog::getDouble(0, tr(""), tr("Z Val"), -1000, -1000, 2000, 1, &ok);
            if(ok)
            {
                cutByZPlane(zval);
                update();
            }
        }
        //        return;
    }
    if(event->key() == Qt::Key_Space && event->modifiers() == Qt::NoModifier)
    {
        getScene()->setDefaultView();
        update();
        return;
    }
    else if(event->key() == Qt::Key_M && event->modifiers() == Qt::NoModifier)
    {
        this->setSelectMode((mode_ + 1) % 3);

    }
    QMesh3D *mesh = getCurrentModel();
    if(mesh)
    {
        double offset = 1;
        if(event->key() == Qt::Key_W && event->modifiers() == Qt::NoModifier)
        {
            this->rotate(QVector3D(offset, 0, 0));
        }
        else if(event->key() == Qt::Key_S && event->modifiers() == Qt::NoModifier)
        {
            this->rotate(QVector3D(-offset, 0, 0));
        }
        else if(event->key() == Qt::Key_A && event->modifiers() == Qt::NoModifier)
        {
            this->rotate(QVector3D(0, offset, 0));
        }
        else if(event->key() == Qt::Key_D && event->modifiers() == Qt::NoModifier)
        {
            this->rotate(QVector3D(0, -offset, 0));
        }
        else if(event->key() == Qt::Key_Q && event->modifiers() == Qt::NoModifier)
        {
            this->rotate(QVector3D(0, 0, offset));
        }
        else if(event->key() == Qt::Key_E && event->modifiers() == Qt::NoModifier)
        {
            this->rotate(QVector3D(0, 0, -offset));
        }
        else if(event->key() == Qt::Key_Left && event->modifiers() == Qt::NoModifier)
        {
            this->translate(QVector3D(-offset, 0, 0));
        }
        else if(event->key() == Qt::Key_Right && event->modifiers() == Qt::NoModifier)
        {
            this->translate(QVector3D(offset, 0, 0));
        }
        else if(event->key() == Qt::Key_Up && event->modifiers() == Qt::NoModifier)
        {
            this->translate(QVector3D(0, offset, 0));
        }
        else if(event->key() == Qt::Key_Down && event->modifiers() == Qt::NoModifier)
        {
            this->translate(QVector3D(0, -offset, 0));
        }
        else if(event->key() == Qt::Key_Up && event->modifiers() == Qt::ControlModifier)
        {
            this->translate(QVector3D(0, 0, offset / 2));
        }
        else if(event->key() == Qt::Key_Down && event->modifiers() == Qt::ControlModifier)
        {
            this->translate(QVector3D(0, 0, -offset / 2));
        }
        else if(event->key() == Qt::Key_1)
        {
            this->applyLeftEuler(QVector3D(0, 0, offset));
        }
        else if(event->key() == Qt::Key_2)
        {
            this->applyLeftEuler(QVector3D(0, 0, -offset));
        }
        else if(event->key() == Qt::Key_9)
        {
            this->translate(QVector3D(0, 0, offset / 2));
        }
        else if(event->key() == Qt::Key_0)
        {
            this->translate(QVector3D(0, 0, -offset / 2));
        }
        else if(event->key() == Qt::Key_R && event->modifiers() == Qt::NoModifier)
        {
            this->rotate(QVector3D(0, 0, 0));
        }
        else if(event->key() == Qt::Key_T && event->modifiers() == Qt::NoModifier)
        {
            this->translate(QVector3D(0, 0, 0));
        }
        else if(event->key() == Qt::Key_C)
        {
            this->slotMoveModelCenter();
        }
        else if(event->key() == Qt::Key_V)
        {
            this->slotUpdateVertices();
        }
        else if(event->key() == Qt::Key_N)
        {
            deleteUnselection();
        }
        else if(event->key() == Qt::Key_Delete)
        {
            if(mode_ == PickPoint)
            {
                removePickPoint();
            }
            else
            {
                removeCurrentModel();
            }
        }
        update();
    }

}

void QModelMakerWidget::mousePressEvent(QMouseEvent *event)
{
    if(event->buttons() == Qt::LeftButton)
    {
//        transform_dialog_->hide();

        if(event->modifiers() == Qt::ControlModifier)
        {
            for(int i = -5; i <= 5; i++)
            {
                for(int j = -5; j <= 5; j++)
                {
                    this->pickAt(event->pos() + QPoint(i, j));
                }
            }
            update();
        }
        else
        {
            if(mode_ == PickPoint)
            {
                this->pickAt(event->pos());
                update();
            }
        }
        rect_p1_ = rect_p0_ = event->pos();
    }
    QRenderWidget::mousePressEvent(event);
}

void QModelMakerWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() == Qt::LeftButton)
    {
        if(event->modifiers() == Qt::ControlModifier)
        {
            for(int i = -5; i <= 5; i++)
            {
                for(int j = -5; j <= 5; j++)
                {
                    this->pickAt(event->pos() + QPoint(i, j), true);
                }
            }
            update();
        }
        else if(mode_ != PickPoint)
        {

            rect_p1_ = event->pos();

            QVector2D v0(rect_p0_.x(), rect_p0_.y());
            QVector2D v1(rect_p1_.x(), rect_p1_.y());
            v0[0] /= this->width();
            v0[1] /= this->height();
            v1[0] /= this->width();
            v1[1] /= this->height();
            v0[1] = 1.0 - v0[1];
            v1[1] = 1.0 - v1[1];
            v0 = v0 * 2.0 - QVector2D(1.0, 1.0);
            v1 = v1 * 2.0 - QVector2D(1.0, 1.0);

            v1 = v1 - v0;
            makeCurrent();
            rect_mesh_->updateRect(v0.x(), v0.y(), v1.x(), v1.y());
            rect_mesh_->setVisible(true);
            update();
        }
        else if(mode_ == PickPoint)
        {

            pickAt(event->pos(), true);
            update();
        }



    }
    QRenderWidget::mouseMoveEvent(event);
}

void QModelMakerWidget::mouseReleaseEvent(QMouseEvent *event)
{
    QRenderWidget::mouseReleaseEvent(event);
    //    if(rect_mesh_->isVisible())
    if(event->button() == Qt::LeftButton)
    {
        if(event->modifiers() != Qt::ControlModifier && mode_ != PickPoint)
        {
            rect_mesh_->setVisible(false);
            QVector2D v0(rect_p0_.x() / float(width()), 1.0 - rect_p0_.y() / float(height()));
            QVector2D v1(rect_p1_.x() / float(width()), 1.0 - rect_p1_.y() / float(height()));

            v0 = v0 * 2.0 - QVector2D(1.0, 1.0);
            v1 = v1 * 2.0 - QVector2D(1.0, 1.0);
            v1 = v1 - v0;
            if(v1[0] < 0)
            {
                v0[0] += v1[0];
                v1[0] = -v1[0];
            }
            if(v1[1] < 0)
            {
                v0[1] += v1[1];
                v1[1] = -v1[1];
            }
            QRectF rect(v0.x(), v0.y(), v1.x(), v1.y());
            selectByRect(rect, mode_);
            update();
        }

    }
    if(event->button() == Qt::MidButton || event->button() == Qt::RightButton)
    {
        saveBuffer();
    }

}

void QModelMakerWidget::wheelEvent(QWheelEvent *e)
{
    QRenderWidget::wheelEvent(e);
    saveBuffer();
}

void QModelMakerWidget::initShader(const QString &vshader_filename, const QString &fshader_filename)
{
    if(material_)
    {
        delete material_;
        material_ = nullptr;
    }
    material_ = new QMaterial(this);
    bool has_error = false;
    if(!has_error && !material_->addShaderFromSourceFile(QOpenGLShader::Vertex, vshader_filename))
    {
        has_error = true;
    }
    // Compile fragment shader
    if (!has_error && !material_->addShaderFromSourceFile(QOpenGLShader::Fragment, fshader_filename))
    {
        has_error = true;
    }
    if (!has_error && !material_->link())
    {
        has_error = true;
    }
    if(has_error)
    {
        material_->deleteLater();
        material_ = nullptr;
    }
}

void QModelMakerWidget::translate(QVector3D offset)
{

    if(mode_ != PickPoint)
    {
        QMesh3D *mesh = getCurrentModel();
        if(!mesh)     return;
        mesh->translate(offset);
        this->v3_dlg_type_ = TM_TranslateModel;
        transform_dialog_->showDialog(tr("Edit Model"), mesh->getOffset(), mesh->getEuler());
    }
    else
    {
        PickPointInfo *pickinfo = getCurrentPickInfo();
        if(!pickinfo) return;
        pickinfo->offset_ += offset;
        this->v3_dlg_type_ = TM_TranslatePickpoint;
        transform_dialog_->showDialog(tr("Edit Pickpoint"), pickinfo->offset_, pickinfo->euler_);
    }


    this->activateWindow();

}

void QModelMakerWidget::rotate(QVector3D euler)
{
    if(mode_ != PickPoint)
    {
        QMesh3D *mesh = getCurrentModel();
        if(!mesh)     return;
        mesh->rotateEuler(euler);
        this->v3_dlg_type_ = TM_RotateModel;
        transform_dialog_->showDialog(tr("Edit Model"), mesh->getOffset(), mesh->getEuler());
    }
    else
    {
        PickPointInfo *pickinfo = getCurrentPickInfo();
        if(!pickinfo) return;
        pickinfo->euler_ += euler;
        this->v3_dlg_type_ = TM_RotatePickpoint;
        transform_dialog_->showDialog(tr("Edit Pickpoint"), pickinfo->offset_, pickinfo->euler_);
    }
    this->activateWindow();

}

void QModelMakerWidget::applyLeftEuler(QVector3D euler)
{
    if(mode_ != PickPoint)
    {
        QMesh3D *mesh = getCurrentModel();
        if(!mesh)     return;
        mesh->applyLeftEuler(euler);
        this->v3_dlg_type_ = TM_RotateModel;
        transform_dialog_->showDialog(tr("Edit Model"), mesh->getOffset(), mesh->getEuler());
    }
    else
    {
        PickPointInfo *pickinfo = getCurrentPickInfo();
        if(!pickinfo) return;
        axes_->setEuler(pickinfo->euler_);
        axes_->applyLeftEuler(euler);
        this->v3_dlg_type_ = TM_RotatePickpoint;
        transform_dialog_->showDialog(tr("Edit Pickpoint"), pickinfo->offset_, axes_->getEuler());
    }
    this->activateWindow();
}

QModelMakerWidget::PickPointInfo *QModelMakerWidget::getCurrentPickInfo()
{
    if(select_pick_id_ < 0 || select_pick_id_ >= pickpoint_list_.size()) return nullptr;
    return &pickpoint_list_[select_pick_id_];
}

void QModelMakerWidget::saveBuffer()
{
    makeCurrent();
    QOpenGLFramebufferObject fbo(this->width(), this->height(), QOpenGLFramebufferObject::CombinedDepthStencil);
    glDisable(GL_BLEND);
    fbo.bind();
    scene_->clearColor(QVector4D(0, 0, 0, 255));

    material_->addUniformValue("u_showtype", int(4));
    scene_->render();
    material_->addUniformValue("u_showtype", int(0));

    fbo.bindDefault();
    info_image_ = fbo.toImage();

    //    info_image_.save("info.png");

}

void QModelMakerWidget::pickAt(QPoint pos, bool isMoving)
{
    makeCurrent();
    if(info_image_.isNull()) return;
    if(!info_image_.rect().contains(pos)) return;
    QRgb rgb = info_image_.pixel(pos);
    QVector4D vv(qRed(rgb), qGreen(rgb), qBlue(rgb), qAlpha(rgb));
    vv /= 255;
    VertexAttachedInfo vinfo;
    vinfo.fromColor(vv);

    if(vinfo.mid_ == current_id_)
    {
        QMesh3D *mesh = getCurrentModel();
        if(!mesh) return;
        if(vinfo.status_ == Invalid || vinfo.status_ >= 4) return;

        if(vinfo.fid_ * 3 >= mesh->array_size() / sizeof(QMesh3D::VertexData)) return;
        if(vinfo.status_ == Deleted) return;
        if(mode_ == PickPoint)
        {
            QMesh3D::VertexData v0 =   mesh->readVertex(vinfo.fid_ * 3);
            QMesh3D::VertexData v1 =   mesh->readVertex(vinfo.fid_ * 3 + 1);
            QMesh3D::VertexData v2 =   mesh->readVertex(vinfo.fid_ * 3 + 2);

            QVector3D raypoint, raydir;
            scene_->getScreenRay(pos.x(), this->height() - pos.y(), raypoint, raydir);
            bool has_intesection = false;
            QVector3D intersect = QMathUtil::getRayTriangleIntesect(raypoint, raydir,
                                  mesh->localTransform(v0.position_),
                                  mesh->localTransform(v1.position_),
                                  mesh->localTransform(v2.position_), &has_intesection);
            intersect += raydir.normalized() * 1.0;
            intersect = mesh->inverseLocalTransform(intersect);



            //            if(has_intesection)
            {
                if(!isMoving)
                {
                    select_pick_id_ = -1;

                    float minlen = -1;
                    for(const auto &pick_info : pickpoint_list_)
                    {
                        QVector3D spos = scene_->project(mesh->localTransform(pick_info.offset_));
                        spos /= spos[2];
                        spos = (spos + QVector3D(1, 1, 0)) * 0.5;
                        QPoint p1(this->width() * spos[0], this->height() * (1.0 - spos[1]));
                        float len = (p1 - pos).manhattanLength();
                        if(len < 30)
                        {

                            if(minlen < 0 || len < minlen)
                            {
                                minlen = len;
                                select_pick_id_ = pick_info.id_;
                            }
                        }
                    }

                    if(select_pick_id_ < 0)
                    {
                        PickPointInfo pickinfo;
                        pickinfo.model_name_ = mesh->objectName();
                        pickinfo.mid_ = vinfo.mid_;
                        pickinfo.fid_ = vinfo.fid_;
                        pickinfo.offset_ = intersect;
                        pickinfo.id_ = pickpoint_list_.size();
                        pickinfo.euler_ = QMathUtil::fromVectorTransformToEuler(QVector3D(0, 0, 1), v0.normal_);
                        select_pick_id_ = pickpoint_list_.size();
                        pickpoint_list_.push_back(pickinfo);
                    }

                }
                else
                {
                    if(select_pick_id_ >= 0 && select_pick_id_ < pickpoint_list_.size())
                    {
                        selectById( pickpoint_list_[select_pick_id_].mid_);
                        pickpoint_list_[select_pick_id_].offset_ = intersect;
                        pickpoint_list_[select_pick_id_].euler_ = QMathUtil::fromVectorTransformToEuler(QVector3D(0, 0, 1), v0.normal_);

                    }
                }
                this->translate(QVector3D(0, 0, 0));
            }



        }
        else
        {
            if(mode_ == Selector)
            {
                vinfo.status_ = Selected;
            }
            else if(mode_ == Eraser)
            {
                vinfo.status_ = Valid;
            }

            QMesh3D::VertexData v0 =   mesh->readVertex(vinfo.fid_ * 3);
            v0.color_ =  vinfo.toColor();
            mesh->writeVertex(vinfo.fid_ * 3, v0);

            v0 =   mesh->readVertex(vinfo.fid_ * 3 + 1);
            v0.color_ =  vinfo.toColor();
            mesh->writeVertex(vinfo.fid_ * 3 + 1, v0);

            v0 =   mesh->readVertex(vinfo.fid_ * 3 + 2);
            v0.color_ =  vinfo.toColor();
            mesh->writeVertex(vinfo.fid_ * 3 + 2, v0);

            this->translate(QVector3D(0, 0, 0));

        }
    }
}

void QModelMakerWidget::VertexAttachedInfo::fromColor(const QVector4D &color)
{
    uint code =  QMaterial::unpackInt(color);

    status_ = code % 4;
    mid_ = (code % 32) / 4;
    fid_ = code / 32;
}

QVector4D QModelMakerWidget::VertexAttachedInfo::toColor(bool *ok) const
{
    int code = 0;
    code += status_ % 4;
    code += (mid_) * 4;
    code += (fid_) * 8 * 4;

    QVector4D v = QMaterial::packInt(code);

    if(ok)
    {
        if(status_ < 0 || mid_ < 0 || fid_ < 0 || mid_ >= 8)
        {
            *ok = false;
        }
        else
        {
            *ok = true;
        }
    }
    return v;

}

QJsonObject QModelMakerWidget::PickPointInfo::toJson() const
{
    QJsonObject obj;
    QFinderConfig::RigidTransform rt;
    rt.euler_ = euler_;
    rt.offset_ = offset_;
    QJsonArray arr;

    rt.toJson(arr);

    obj["pick"] = arr;

    rt.euler_ = mesh_euler_;
    rt.offset_ = mesh_offset_;
    rt.toJson(arr);

    obj["mesh"] = arr;


    rt.euler_ = place_euler_;
    rt.offset_ = place_offset_;
    rt.toJson(arr);
    obj["place"] =  arr;

    return obj;
}

QJsonObject QModelMakerWidget::PickPointInfo::toSimpleJson() const
{
    QJsonObject obj;
    QFinderConfig::RigidTransform rt;
    rt.euler_ = euler_;
    rt.offset_ = offset_;
    QJsonArray arr;
    rt.toJson(arr);
    obj["point"] = arr;
    return obj;
}

}
