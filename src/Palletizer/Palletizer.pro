QT +=core gui widgets opengl websockets
DESTDIR = ../../bin


SUBDIRS += \
    Palletizer.pro

HEADERS += \
    Palletizer.h \
    QueryBox.h \
    RectNode.h \
    SolverServer.h \
    QPalletizerWidget.h

SOURCES += \
    Palletizer.cpp \
    QueryBox.cpp \
    RectNode.cpp \
    main.cpp \
    SolverServer.cpp \
    QPalletizerWidget.cpp

win32{
LIBS += -L$$PWD/../../dll_backup/win/q3dlib/ -lq3d_gcl
}
unix
{
LIBS += -L$$PWD/../../dll_backup/linux/q3dlib/ -lq3d_gcl
}

INCLUDEPATH += $$PWD/../../ \
               $$PWD/../Q3D
DEPENDPATH += $$PWD/../../
