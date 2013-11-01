#-------------------------------------------------
#
# Project created by QtCreator 2013-11-01T21:23:55
#
#-------------------------------------------------

QT       += core

TEMPLATE = app

DESTDIR = ../../Binary/

SOURCES += main.cpp


win32: LIBS += -L../../Binary/ -lDebugFileLogger

INCLUDEPATH += ../../DebugFileLogger/src

win32: {
    LIBS += -lshell32 \
            -lAdvapi32

}
