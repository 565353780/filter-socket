QT +=core gui widgets opengl
DESTDIR = ../../bin
SOURCES += \
    main.cpp \
    PointMapWidget.cpp

win32{
LIBS += -L$$PWD/../../dll_backup/win/q3dlib/ -lq3d_gcl
}
unix
{
LIBS += -L$$PWD/../../dll_backup/linux/q3dlib/ -lq3d_gcl
}

INCLUDEPATH += $$PWD/../../ \
               $$PWD/../Q3D \
                $$PWD/../Q3D/Core
DEPENDPATH += $$PWD/../../

HEADERS += \
    PointMapWidget.h
