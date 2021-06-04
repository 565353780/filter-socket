win32{
DESTDIR = ../../bin_win
}
unix{
DESTDIR = ../../bin_linux
}

SOURCES += main.cpp

win32{
DEFINES += WIN32
}
unix{
DEFINES += Linux
}
