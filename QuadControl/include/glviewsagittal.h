#pragma once
#include <QQuickFramebufferObject>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QHoverEvent>
#include <glm/glm.hpp>
#include <glviewsagittalrenderer.h>

class GLViewSagittal : public QQuickFramebufferObject
{
    Q_OBJECT
    QML_ELEMENT

public:
    GLViewSagittal()
    {
        setAcceptedMouseButtons(Qt::AllButtons);  // enable mouse buttons
        setAcceptHoverEvents(true);               // enable hover events
        setFocusPolicy(Qt::StrongFocus);          // optional, for keyboard

        minVoxelThresholdValue = 0;
        maxVoxelThresholdValue = 65000;
    }

    Renderer* createRenderer() const override 
    {
        Context context(R"(D:\Files\Cesars\Scissors_Test 2025-7-2 15-11-21.uint16_scv)");
        context.volumeData.saveHeaderToFile("volumeHeader.txt");
        context.volumeData.fillBuffer();

        qDebug() << "GLViewSagittal";

        return new GLViewSagittalRenderer(Qt::blue, &context);

    }
protected:
    void mouseMoveEvent(QMouseEvent* event) override
    {
        Q_UNUSED(event);
    }

    void mousePressEvent(QMouseEvent* event) override
    {
        Q_UNUSED(event);
    }

    void mouseReleaseEvent(QMouseEvent* event) override
    {
        Q_UNUSED(event);
    }

    void keyPressEvent(QKeyEvent* event) override
    {
        Q_UNUSED(event);
    }

    void hoverEnterEvent(QHoverEvent* event) override
    {
        Q_UNUSED(event);
    }

    void hoverLeaveEvent(QHoverEvent* event) override
    {
        Q_UNUSED(event);
    }

    void wheelEvent(QWheelEvent* event) override
    {
        int deltaY = event->angleDelta().y();
        yDistance += deltaY * 0.00005f;
        yDistance = __max(0.0, __min(1.0f, yDistance));
        update();
    }
private:
    float minVoxelThresholdValue;
    float maxVoxelThresholdValue;
    float yDistance;
};