#include "QPalletizerWidget.h"
#include <QApplication>
#include <QDebug>
int main(int argc, char **argv)
{
    QApplication a(argc,argv);
    GCL::QPalletizerWidget w;
    w.resize(800,600);
    w.show();
    w.slotLoadTestQueue("palletizer.p.json");
    return a.exec();
}
