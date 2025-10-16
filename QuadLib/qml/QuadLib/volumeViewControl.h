#pragma once
#include <QQuickFramebufferObject>
#include "GLRenderer.h"

class VolumeViewControl : public QQuickFramebufferObject {
    Q_OBJECT
public:
    Renderer* createRenderer() const override {
        return new GLRenderer();
    }
};