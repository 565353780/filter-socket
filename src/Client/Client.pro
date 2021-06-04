QT += core gui network widgets sql

TEMPLATE = app

win32{
DESTDIR = ../../bin_win
}
unix{
DESTDIR = ../../bin_linux
}

win32{
DEFINES += WIN32
}
unix{
DEFINES += Linux
}

HEADERS += ../Connection/Connection.h \
           ClientDialog.h \
           MaskBBoxInfoWidget.h \
           MaskWidget.h \
           CameraMaskWidget.h \
           MaskPolyWidget.h \
           MaskDrawWidget.h \
           MaskDatasetControlWidget.h \
           DatabaseManager/DatabaseManager.h \
           DataManager.h \
           DataManagerWidget.h \
           DataLoader.h \
           ImageFile.h

SOURCES += main.cpp \
           ../Connection/Connection.cpp \
           ClientDialog.cpp \
           MaskBBoxInfoWidget.cpp \
           MaskWidget.cpp \
           CameraMaskWidget.cpp \
           MaskPolyWidget.cpp \
           MaskDrawWidget.cpp \
           MaskDatasetControlWidget.cpp \
           DatabaseManager/DatabaseManager.cpp \
           DataManager.cpp \
           DataManagerWidget.cpp \
           DataLoader.cpp \
           ImageFile.cpp

INCLUDEPATH += ../MaskConfig \
               ../../thirdparty/eigen-3.3.7 \
               ../../thirdparty/Eigen3.3.5

HEADERS += \
    ../MaskConfig/Config/Setting/Setting.h \
    ../MaskConfig/Config/Setting/NumSetting.h \
    ../MaskConfig/Config/Setting/PolygonSetting.h \
    ../MaskConfig/Util/LogTool.h \
    ../MaskConfig/Config/Setting/SettingWords.h \
    ../MaskConfig/Config/Config.h \
    ../MaskConfig/Config/Setting/PolygonListSetting.h \
    ../MaskConfig/Config/Setting/SettingFunctions.h \
    ../MaskConfig/Config/Setter/Setter.h \
    ../MaskConfig/Controller/Controller.h \
    ../MaskConfig/Config/Setter/PolygonSetter.h \
    ../MaskConfig/Config/Setter/SetterCommands.h \
    ../MaskConfig/WIdgets/ConfigWidget.h \
    ../MaskConfig/WIdgets/PaintWidget.h \
    ../MaskConfig/WIdgets/QLogTool.h \
    ../MaskConfig/Config/Setter/PolygonsSetter.h \
    ../MaskConfig/Config/Setting/LabelDict.h \
    ../MaskConfig/WIdgets/LabelDictWidget.h \
    ../MaskConfig/WIdgets/SetterWidget.h \
    ../MaskConfig/WIdgets/PanelTabDialog.h \
    ../MaskConfig/WIdgets/SettingRowWidget.h \
    ../MaskConfig/Config/Setting/SelectSetting.h \
    ../MaskConfig/WIdgets/CategoryWidget.h \
    ../MaskConfig/WIdgets/InfoWidget.h \
    ../MaskConfig/WIdgets/BindingWidget.h \
    ../MaskConfig/Config/Setter/PolygonsBinding.h \
    ../MaskConfig/Config/Dict.h \
    ../MaskConfig/Config/Setting/BBoxSetting.h \
    ../MaskConfig/Config/Setter/BBoxSetter.h \
    ../MaskConfig/WIdgets/TrackIdWidget.h \
    ../MaskConfig/Config/Setting/TextSetting.h \
    ../MaskConfig/WIdgets/ProgressBarWidget.h

SOURCES += \
    ../MaskConfig/Config/Setting/Setting.cpp \
    ../MaskConfig/Config/Setting/NumSetting.cpp \
    ../MaskConfig/Config/Setting/PolygonSetting.cpp \
    ../MaskConfig/Util/LogTool.cpp \
    ../MaskConfig/Config/Config.cpp \
    ../MaskConfig/Config/Setting/PolygonListSetting.cpp \
    ../MaskConfig/Config/Setting/SettingFunctions.cpp \
    ../MaskConfig/Config/Setter/Setter.cpp \
    ../MaskConfig/Controller/Controller.cpp \
    ../MaskConfig/Config/Setter/PolygonSetter.cpp \
    ../MaskConfig/Config/Setter/SetterCommands.cpp \
    ../MaskConfig/WIdgets/ConfigWidget.cpp \
    ../MaskConfig/WIdgets/PaintWidget.cpp \
    ../MaskConfig/WIdgets/QLogTool.cpp \
    ../MaskConfig/Config/Setter/PolygonsSetter.cpp \
    ../MaskConfig/Config/Setting/LabelDict.cpp \
    ../MaskConfig/WIdgets/LabelDictWidget.cpp \
    ../MaskConfig/WIdgets/SetterWidget.cpp \
    ../MaskConfig/WIdgets/PanelTabDialog.cpp \
    ../MaskConfig/WIdgets/SettingRowWidget.cpp \
    ../MaskConfig/Config/Setting/SelectSetting.cpp \
    ../MaskConfig/WIdgets/CategoryWidget.cpp \
    ../MaskConfig/WIdgets/InfoWidget.cpp \
    ../MaskConfig/WIdgets/BindingWidget.cpp \
    ../MaskConfig/Config/Setter/PolygonsBinding.cpp \
    ../MaskConfig/Config/Dict.cpp \
    ../MaskConfig/Config/Setting/BBoxSetting.cpp \
    ../MaskConfig/Config/Setter/BBoxSetter.cpp \
    ../MaskConfig/WIdgets/TrackIdWidget.cpp \
    ../MaskConfig/Config/Setting/TextSetting.cpp \
    ../MaskConfig/WIdgets/ProgressBarWidget.cpp

win32{
LIBS += \
    -L"C:\Program Files\MySQL\MySQL Server 8.0\lib" \
    -llibmysql
}

FORMS += clientdialog.ui \
         maskbboxinfowidget.ui \
         maskdatasetcontrolwidget.ui \
         maskpolywidget.ui \
         ../MaskConfig/WIdgets/progressbarwidget.ui \
         datamanagerwidget.ui

TRANSLATIONS += tt_client.ts
