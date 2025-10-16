#include "volumeViewControl.h"
#include <QOpenGLFunctions>
#include <QDebug>
#include "glrenderer.h"
#include "volumeViewControl.h"
#include <QOpenGLFunctions>
#include <qopenglframebufferobject.h>

QQuickFramebufferObject::Renderer* VolumeViewControl::createRenderer() const {
    return new GLRenderer();
}

