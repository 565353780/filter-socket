TEMPLATE = app
CONFIG -= console
QT += core gui widgets
DESTDIR = ../../bin

win32{
DEFINES += WIN32
}
unix{
DEFINES += Linux
}

HEADERS += \
    Config/Setting/Setting.h \
    Config/Setting/NumSetting.h \
    Config/Setting/PolygonSetting.h \
    Util/LogTool.h \
    Config/Setting/SettingWords.h \
    Config/Config.h \
    Config/Setting/PolygonListSetting.h \
    Config/Setting/SettingFunctions.h \
    Config/Setter/Setter.h \
    Controller/Controller.h \
    Config/Setter/PolygonSetter.h \
    Config/Setter/SetterCommands.h \
    WIdgets/ConfigWidget.h \
    WIdgets/PaintWidget.h \
    WIdgets/QLogTool.h \
    Config/Setter/PolygonsSetter.h \
    Config/Setting/LabelDict.h \
    WIdgets/LabelDictWidget.h \
    WIdgets/SetterWidget.h \
    WIdgets/PanelTabDialog.h \
    WIdgets/SettingRowWidget.h \
    Config/Setting/SelectSetting.h \
    WIdgets/CategoryWidget.h \
    WIdgets/InfoWidget.h \
    WIdgets/BindingWidget.h \
    Config/Setter/PolygonsBinding.h \
    Config/Dict.h \
    Config/Setting/BBoxSetting.h \
    Config/Setter/BBoxSetter.h \
    WIdgets/TrackIdWidget.h \
    Config/Setting/TextSetting.h \
    WIdgets/ProgressBarWidget.h

SOURCES += \
    Config/Setting/Setting.cpp \
    Config/Setting/NumSetting.cpp \
    Config/Setting/PolygonSetting.cpp \
    Util/LogTool.cpp \
    main.cpp \
    Config/Config.cpp \
    Config/Setting/PolygonListSetting.cpp \
    Config/Setting/SettingFunctions.cpp \
    Config/Setter/Setter.cpp \
    Controller/Controller.cpp \
    Config/Setter/PolygonSetter.cpp \
    Config/Setter/SetterCommands.cpp \
    WIdgets/ConfigWidget.cpp \
    WIdgets/PaintWidget.cpp \
    WIdgets/QLogTool.cpp \
    Config/Setter/PolygonsSetter.cpp \
    Config/Setting/LabelDict.cpp \
    WIdgets/LabelDictWidget.cpp \
    WIdgets/SetterWidget.cpp \
    WIdgets/PanelTabDialog.cpp \
    WIdgets/SettingRowWidget.cpp \
    Config/Setting/SelectSetting.cpp \
    WIdgets/CategoryWidget.cpp \
    WIdgets/InfoWidget.cpp \
    WIdgets/BindingWidget.cpp \
    Config/Setter/PolygonsBinding.cpp \
    Config/Dict.cpp \
    Config/Setting/BBoxSetting.cpp \
    Config/Setter/BBoxSetter.cpp \
    WIdgets/TrackIdWidget.cpp \
    Config/Setting/TextSetting.cpp \
    WIdgets/ProgressBarWidget.cpp


TRANSLATIONS += ch.ts

FORMS += \
    WIdgets/progressbarwidget.ui
