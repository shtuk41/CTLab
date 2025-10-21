#pragma once
#include <QQuickFramebufferObject>
#include <glviewsagittalrenderer.h>

class GLViewSagittal : public QQuickFramebufferObject
{
    Q_OBJECT
    QML_ELEMENT

public:
    Renderer* createRenderer() const override {

        qDebug() << "GLViewSagittal";

        return new GLViewSagittalRenderer();
    }
};