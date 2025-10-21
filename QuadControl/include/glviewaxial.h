#pragma once
#include <QQuickFramebufferObject>
#include <glviewaxialrenderer.h>

class GLViewAxial : public QQuickFramebufferObject
{
    Q_OBJECT
    QML_ELEMENT

public:
    Renderer* createRenderer() const override {

        qDebug() << "GLViewAxial";

        return new GLViewAxialRenderer();
    }
};