QT += quick qml opengl
TEMPLATE = lib
CONFIG += plugin
TARGET = QuadControl
DESTDIR = $$OUT_PWD/qml/QuadControl

SOURCES += \
    plugin.cpp
	
QML_PLUGIN_METADATA = quadcontrol.json	