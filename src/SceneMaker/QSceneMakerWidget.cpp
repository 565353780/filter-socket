#include "QSceneMakerWidget.h"
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

#include <ctime>
namespace GCL {

QSceneMakerWidget::QSceneMakerWidget(QWidget *parent):QRenderWidget(parent)
{
    scene_->setRotationSpeed(2);
    scene_->setPanSpeed(40);
    scene_->setScaleSpeed(1);
    scene_->setPanSpeed(20);
    scene_->setProjectionParameters(0.1,10000.0,45);
    this->show();
}
void QSceneMakerWidget::initShader(const QString &vshader_filename, const QString &fshader_filename)
{
    if(material_){
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

void QSceneMakerWidget::loadModel(const QString &filename)
{
    QMesh3D *mesh = nullptr;
    if(material_)
    {
        mesh = new QMesh3D(material_, scene_);
    }
    else
    {
        mesh = new QMesh3D(scene_);
    }
    mesh->loadFile(filename,true);
    if(mesh->array_size() == 0)
    {
        mesh->deleteLater();
    }
    else
    {
        recent_filename_ = filename;
        if(getCurrentModel())
        {
            getCurrentModel()->setSelected(false);
        }

        current_id_ = this->model_list_.size();
        this->model_list_.push_back(mesh);
        mesh->setSelected(true);
        mesh->setColor(QVector4D(0.1,1.0,0.5,1.0));
        update();

    }
}

QMesh3D *QSceneMakerWidget::getCurrentModel()
{
    if(current_id_>=0 && current_id_ < model_list_.size())
    {
        return model_list_[current_id_];
    }
    else
    {
        return nullptr;
    }
}

void QSceneMakerWidget::remove_model()
{
    QMesh3D *mesh = getCurrentModel();
    if(!mesh) return;
    mesh->deleteLater();
    model_list_.removeAt(current_id_);
    current_id_ = -1;

}

void QSceneMakerWidget::selectById(int id)
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
    current_id_ = id;

    QMesh3D *mesh = getCurrentModel();
    if(mesh)
    {
        mesh->setSelected(true);
    }


}

void QSceneMakerWidget::randomMake()
{
    QMesh3D *mesh = getCurrentModel();
    if(!mesh) return;
    QVector3D vmax,vmin;
    vmax = mesh->get_bbox_vmax();
    vmin = mesh->get_bbox_vmin();

    double ll = (vmax - vmin).length() * 0.5;
    srand(clock());


    QVector3D vpos( rand() % 360 - 180, rand() % 360 - 180, ll * ((rand() % 100) /150.0 + 0.4));

    QVector3D veuler(rand() % 360, rand() % 360, rand() % 360);

    mesh->setOffset(vpos);
    mesh->setEuler(veuler);


}

void QSceneMakerWidget::save_map()
{
    QOpenGLFramebufferObject fbo(this->width(),this->height(),QOpenGLFramebufferObject::CombinedDepthStencil);

    fbo.bind();
    scene_->clearColor(QVector4D(0,0,0,0));

    material_->addUniformValue("u_showtype",1);
    scene_->render();
    material_->addUniformValue("u_showtype",0);

    fbo.bindDefault();

    z_image_ = fbo.toImage();

    fbo.bind();
    scene_->clearColor(QVector4D(0,0,0,0));

    material_->addUniformValue("u_showtype",2);
    scene_->render();
    material_->addUniformValue("u_showtype",0);

    fbo.bindDefault();

    x_image_ = fbo.toImage();

    fbo.bind();
    scene_->clearColor(QVector4D(0,0,0,0));

    material_->addUniformValue("u_showtype",3);
    scene_->render();
    material_->addUniformValue("u_showtype",0);

    fbo.bindDefault();

    y_image_ = fbo.toImage();


    QList< QList<QVector3D> > points;
    for(int j=0; j < z_image_.height(); j++)
    {
        QList<QVector3D> row;
        for(int i=0; i < z_image_.width(); i++)
        {
            QRgb rgb =z_image_.pixel(i,j);
            QVector4D vv(qRed(rgb),qGreen(rgb),qBlue(rgb),qAlpha(rgb));
            vv /= 255;
            qreal zval = QMaterial::unpackInt(vv) / scene_->getPackScale() + scene_->getPackMinval();

            rgb =x_image_.pixel(i,j);
            vv  = QVector4D(qRed(rgb),qGreen(rgb),qBlue(rgb),qAlpha(rgb));
            vv /= 255;
            qreal xval = QMaterial::unpackInt(vv) / scene_->getPackScale() + scene_->getPackMinval();

            rgb =y_image_.pixel(i,j);
            vv  = QVector4D(qRed(rgb),qGreen(rgb),qBlue(rgb),qAlpha(rgb));
            vv /= 255;
            qreal yval = QMaterial::unpackInt(vv) / scene_->getPackScale() + scene_->getPackMinval();

            QVector3D pos(xval,
                          yval,zval + rand() % 1000 / 1000.0 - 0.5);
            row.push_back(pos);
        }
        points.push_back(row);
    }
    QMap<int,int> vmap;
    QList<QVector3D> vlist;
    for(int j=1; j < z_image_.height()-1; j++)
    {
        for(int i=1; i < z_image_.width()-1; i++)
        {
            QVector3D p = points[j][i];
            int index = j * z_image_.width() + i;
            if(p.z() < 100000)
            {
                vmap[index] = vlist.size();
                vlist.push_back(p);
            }
        }
    }

    QList<QVector3D> flist;
    for(int j=1; j < z_image_.height()-1; j++)
    {
        for(int i=1; i < z_image_.width()-1; i++)
        {
            int f0 = j * z_image_.width() + i;
            int f1 = j * z_image_.width() + i + 1;
            int f2 = (j+1) * z_image_.width() + i + 1;
            int f3 = (j+1) * z_image_.width() + i;


            if(vmap.count(f0) && vmap.count(f1) && vmap.count(f2))
            {
                if((vlist[vmap[f0]] - vlist[vmap[f1]]).length() > 5) continue;
                if((vlist[vmap[f0]] - vlist[vmap[f2]]).length() > 5) continue;
                if((vlist[vmap[f2]] - vlist[vmap[f1]]).length() > 5) continue;


                flist.push_back(QVector3D(vmap[f1],vmap[f0],vmap[f2]));
            }

            if(vmap.count(f0) && vmap.count(f2) && vmap.count(f3))
            {
                if((vlist[vmap[f0]] - vlist[vmap[f2]]).length() > 5) continue;
                if((vlist[vmap[f0]] - vlist[vmap[f3]]).length() > 5) continue;
                if((vlist[vmap[f2]] - vlist[vmap[f3]]).length() > 5) continue;

                flist.push_back(QVector3D(vmap[f2],vmap[f0],vmap[f3]));
            }
        }
    }
    FILE *fp = fopen("test.obj","w");
    if(!fp) return;


    for(int i=0; i < vlist.size(); i++)
    {
        fprintf(fp,"v %f %f %f\n", vlist[i][0],vlist[i][1],vlist[i][2]);
    }

    for(int i=0; i <flist.size(); i++)
    {
        fprintf(fp,"f %d %d %d\n", int(flist[i][0]+1),int(flist[i][1]+1),int(flist[i][2]+1));
    }
    fclose(fp);




}

void QSceneMakerWidget::initializeGL()
{
    QRenderWidget::initializeGL();

    this->initShader(":/shaders/scenemaker_vshader.glsl",":/shaders/scenemaker_fshader.glsl");
    QMatrix4x4 matrix;
    matrix.lookAt(QVector3D(0,0,500),QVector3D(0,0,0),QVector3D(0,1,0));
    scene_->setDefaultModelMatrix(matrix);    
    scene_->setDefaultView();

    QQuadMesh *quad = new QQuadMesh(material_,scene_);
    quad->init();
    quad->setScale(QVector3D(200,200,1));
    quad->translate(QVector3D(0,0,0));
    quad->setColor(QVector4D(0.8,0.8,0.8,1.0));
}

void QSceneMakerWidget::resizeGL(int w, int h)
{
    QRenderWidget::resizeGL(w,h);

}

void QSceneMakerWidget::paintGL()
{
    QRenderWidget::paintGL();
}

void QSceneMakerWidget::keyPressEvent(QKeyEvent *event)
{
    if(event->modifiers() == Qt::ControlModifier)
    {
        if(event->key() == Qt::Key_O)
        {
            QString filename = QFileDialog::getOpenFileName(0,tr(""),"./","*.obj");
            if(filename.isEmpty()) return;

            this->loadModel(filename);
        }
        else if(event->key() == Qt::Key_I)
        {
            this->loadModel(recent_filename_);
        }
        else if(event->key() == Qt::Key_S)
        {
            qDebug()<<"saving depth...";
            save_map();
            qDebug()<<"saving depth.";

        }
        else if(event->key() ==Qt::Key_R)
        {
            this->loadModel(recent_filename_);
            randomMake();
            update();
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
        return;
    }
    if(event->key() == Qt::Key_Space)
    {
        getScene()->setDefaultView();
        update();
        return;
    }
    QMesh3D *mesh = getCurrentModel();
    if(mesh)
    {
        double offset = 5;
        if(event->key() == Qt::Key_W)
        {
            mesh->rotateEuler(QVector3D(offset,0,0));
        }
        else if(event->key() == Qt::Key_S)
        {
            mesh->rotateEuler(QVector3D(-offset,0,0));
        }
        else if(event->key() == Qt::Key_A)
        {
            mesh->rotateEuler(QVector3D(0,offset,0));
        }
        else if(event->key() == Qt::Key_D)
        {
            mesh->rotateEuler(QVector3D(0,-offset,0));
        }
        else if(event->key() == Qt::Key_Q)
        {
            mesh->rotateEuler(QVector3D(0,0,offset));
        }
        else if(event->key() == Qt::Key_E)
        {
            mesh->rotateEuler(QVector3D(0,0,-offset));
        }
        else if(event->key() == Qt::Key_Left)
        {
            mesh->translate(QVector3D(-offset,0,0));

        }
        else if(event->key() == Qt::Key_Right)
        {
            mesh->translate(QVector3D(offset,0,0));
        }
        else if(event->key() == Qt::Key_Up)
        {
            mesh->translate(QVector3D(0,offset,0));
        }
        else if(event->key() == Qt::Key_Down)
        {
            mesh->translate(QVector3D(0,-offset,0));
        }

        else if(event->key() == Qt::Key_9)
        {
            mesh->translate(QVector3D(0,0,offset/2));
        }
        else if(event->key() == Qt::Key_0)
        {
            mesh->translate(QVector3D(0,0,-offset/2));
        }        
        else if(event->key() == Qt::Key_P)
        {
            qDebug()<<scene_->getModelMatrix();
        }
        else if(event->key() ==Qt::Key_R)
        {
            randomMake();
            update();
        }
        update();
    }

}

void QSceneMakerWidget::mousePressEvent(QMouseEvent *event)
{

    if(event->buttons() == Qt::LeftButton && event->modifiers()==Qt::ControlModifier)
    {
//        if(z_image_.width() > event->x() && z_image_.height() > event->y())
//        {
//             QRgb rgb =z_image_.pixel(event->pos());
//             QVector4D vv(qRed(rgb),qGreen(rgb),qBlue(rgb),qAlpha(rgb));
//             vv /= 255;
//             qreal zval = QMaterial::unpackInt(vv) / scene_->getPackScale() + scene_->getPackMinval();

//             rgb =x_image_.pixel(event->pos());
//             vv  = QVector4D(qRed(rgb),qGreen(rgb),qBlue(rgb),qAlpha(rgb));
//             vv /= 255;
//             qreal xval = QMaterial::unpackInt(vv) / scene_->getPackScale() + scene_->getPackMinval();

//             rgb =y_image_.pixel(event->pos());
//             vv  = QVector4D(qRed(rgb),qGreen(rgb),qBlue(rgb),qAlpha(rgb));
//             vv /= 255;
//             qreal yval = QMaterial::unpackInt(vv) / scene_->getPackScale() + scene_->getPackMinval();

//             QVector3D pos(xval,yval,zval);
//             qDebug()<<pos;
//        }


        QMesh3D *mesh = this->getCurrentModel();
        if(!mesh) return;

        makeCurrent();
//        clock_t t0 = clock();
//        QMesh3D::VertexData vdata = mesh->readVertex(0);

//        vdata.position_ = QVector3D();

//        mesh->writeVertex(0,vdata);

//        qDebug()<<clock() - t0<<"ms";
    }

   else if(event->buttons() == Qt::LeftButton)
   {
       QVector3D planePos(0,0,-200);
       QVector3D planeNormal(0,0,1);


       QVector3D intersect = scene_->pickAtPlane(event->x(),this->height() - event->y(),planePos,planeNormal);

       QMesh3D *mesh = this->getCurrentModel();
       if(!mesh) return;

       QVector3D vmin,vmax;
       mesh->getTransformedBBox(vmin,vmax);
       QVector3D offset = mesh->getOffset();
       QVector3D  vdiff = (vmax-vmin) * 0.5;
        vdiff[0] = 0;
        vdiff[1] = 0;
        QVector3D vcenter = (vmax+vmin) * 0.5;
       mesh->setOffset(offset- vcenter + intersect + vdiff);
       update();
   }

   else
   {
      QRenderWidget::mousePressEvent(event);
   }

}


}
