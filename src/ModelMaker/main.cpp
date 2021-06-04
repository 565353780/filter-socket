#include "QModelMakerWidget.h"
#include <QApplication>
#include <QMatrix4x4>
#include "Core/Q3DScene.h"
#include "Core/QMesh3D.h"
#include "Meshes/QPrimitiveMesh.h"
#include "QModelMakerWindow.h"
#include <QDebug>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    using namespace GCL;
    GCL::QModelMakerWindow w;
    w.show();
    w.resize(1000,800);


    return a.exec();
}
