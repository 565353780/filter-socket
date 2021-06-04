
#include <QApplication>
#include <QLabel>
#include <QSurfaceFormat>

#include <QOpenGLContext>
#include <QOffscreenSurface>
#include "Core/Q3DScene.h"
#include "Meshes/QCubeMesh.h"
#include "Core/QMaterial.h"
#include <QWidget>
#include <QOpenGLFramebufferObject>
#include <QDebug>
#include "Widgets/QRenderWidget.h"
#include "Meshes/QQuadMesh.h"
#include "Meshes/QPointMapMesh.h"
#include "Evaluator/QShaderEvaluator.h"
#include <QImage>
#include <ctime>
#include "Widgets/QModelFinderWidget.h"
#include <QDir>
#include <QFileInfo>
#include "Meshes/QPrimitiveMesh.h"
#include "Finder/QFinderConfig.h"
#include "Core/QMathUtil.h"

int testOnScreen(QString filename, QString cfg_filename = "finder.json")
{
    using namespace GCL;
    GCL::QModelFinderWidget rw;
    int w = 1280 * 0.85;
    int h = 1024 * 0.85;

    rw.loadConfig(cfg_filename);
    rw.loadPointMapPLY(filename, -1);
    rw.resize(w, h);


    return QApplication::exec();
}
int testOffScreen(QString filename, QString cfg_filename = "finder.json")
{
    using namespace GCL;
    clock_t t0 = clock();

    // 1、设定屏幕宽、高
    int w, h;
    w = 1280 * 0.85;
    h = 1024 * 0.85;
    // 2、Finder初始化
    QModelFinder rw;
    rw.create();
    rw.init();



    for(int i = 0; i < 1000; i++)
    {

        rw.loadConfig(cfg_filename);
        rw.loadPointMapPLY(filename, -1);
        //  find models
        rw.findModels();
        // 以下只是为了保存图片用


        rw.resize(w, h);

        QOpenGLFramebufferObject fbo(QSize(w, h), QOpenGLFramebufferObject::CombinedDepthStencil);

        fbo.bind();

        rw.render();

        fbo.bindDefault();
//        fbo.toImage(true).save("testOff.png");

    }

    return 0;



}

int main(int argc, char *argv[])
{

    using namespace GCL;
    QSurfaceFormat format;
//    format.setRenderableType(QSurfaceFormat::OpenGLES);

    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);
    format.setSamples(4);
    QSurfaceFormat::setDefaultFormat(format);
    qDebug() << QSurfaceFormat::defaultFormat().version();


    QApplication app(argc, argv);

    QString filename = "12.ply";
    QString cfg_filename = "finder.json";
    QFile file("testfilename.txt");
    if(file.open((QFile::ReadOnly)))
    {
        QString text = file.readAll();
        text = text.replace("\r", "");
        QStringList list = text.split("\n");
        filename = list[0];
        if(list.size() > 1)
        {
            cfg_filename = list[1];
        }
        file.close();
    }
//    QWidget w;
//    w.show();
//    testOffScreen(filename, cfg_filename);
    testOnScreen(filename, cfg_filename);
    return 0;

}
