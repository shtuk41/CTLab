#pragma once
#include <QQuickFramebufferObject>
#include "GLRenderer.h"

class VolumeViewControl : public QQuickFramebufferObject {
    Q_OBJECT
    QML_ELEMENT

public:
    Renderer* createRenderer() const override {

        qDebug() << "VolumeViewControl";

        return new GLRenderer();
    }
};