#include <QApplication>
#include <QMatrix4x4>
#include "Core/Q3DScene.h"
#include "Core/QMesh3D.h"
#include "Meshes/QPrimitiveMesh.h"
#include "QSceneMakerWidget.h"
#include <QDebug>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    using namespace GCL;
    GCL::QSceneMakerWidget w;
    w.show();
    w.resize(640,512);

    return a.exec();
}
