#pragma once
#include <QQuickFramebufferObject>
#include <glviewcoronalrenderer.h>

class GLViewCoronal : public QQuickFramebufferObject
{
    Q_OBJECT
    QML_ELEMENT

public:
    Renderer* createRenderer() const override {

        qDebug() << "GLViewCoronal";

        return new GLViewCoronalRenderer();
    }
};