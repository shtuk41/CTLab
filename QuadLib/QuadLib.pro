QT += quick qml opengl
TEMPLATE = lib
CONFIG += plugin
TARGET = QuadLib
DESTDIR = $$OUT_PWD/qml/QuadLib

HEADERS += \
    glrenderer.h \
    volumeViewControl.h

SOURCES += \
    plugin.cpp