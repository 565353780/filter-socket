#include "Config/Setting/PolygonSetting.h"
#include "Config/Setting/PolygonListSetting.h"
#include "Config/Setting/NumSetting.h"
#include "Config/Config.h"
#include <QJsonDocument>
#include <QApplication>
#include "Util/LogTool.h"
#include "WIdgets/ConfigWidget.h"
#include <QTranslator>
#include <QDir>
int main(int argc, char **argv)
{
    using namespace MaskConfig;
//    LogTool tool;
//    Config config;


//    config.add("Mask", std::shared_ptr<Setting>(new PolygonSetting()));
//    config.add("Masks", std::shared_ptr<Setting>(new PolygonListSetting()));
//    config.add("Num0",  std::shared_ptr<Setting>(new NumSetting()));

//    config.save("test.json");

//    config.init("test.json");

//    config.save("test2.json");

    QApplication a(argc,argv);
    QDir::setCurrent(a.applicationDirPath());
    QTranslator trans;
    trans.load("ch.qm");
    a.installTranslator(&trans);
    ConfigWidget w;
    w.resize(1280,768);
    w.show();

    return a.exec();
}
