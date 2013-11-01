
TEMPLATE = lib

CONFIG += static

INCLUDEPATH+= src/ \

DESTDIR = ../Binary/

QMAKE_CXXFLAGS_RELEASE = $$QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO
QMAKE_LFLAGS_RELEASE = $$QMAKE_LFLAGS_RELEASE_WITH_DEBUGINFO


DEFINES += CALL_TRACK_ENABLED

HEADERS += \
    src/DebugFileLogger.h \
    src/DebugHelper.h \
    src/DebugFileLoggerRunable.h

SOURCES += \
    src/DebugFileLogger.cpp \
