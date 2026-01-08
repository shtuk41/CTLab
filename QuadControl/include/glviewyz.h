#pragma once
#include <QQuickFramebufferObject>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QHoverEvent>
#include <glm/glm.hpp>
#include <contextWrapper.h>
#include <glviewyzrenderer.h>

class GLViewYZ : public QQuickFramebufferObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(int minVoxelThreshold READ minVoxelThreshold WRITE setMinVoxelThreshold)
    Q_PROPERTY(int maxVoxelThreshold READ maxVoxelThreshold WRITE setMaxVoxelThreshold)
    Q_PROPERTY(ContextWrapper* context READ context WRITE setContext NOTIFY contextChanged)

public:
    GLViewYZ()
    {
        setAcceptedMouseButtons(Qt::AllButtons);  // enable mouse buttons
        setAcceptHoverEvents(true);               // enable hover events
        setFocusPolicy(Qt::StrongFocus);          // optional, for keyboard

        minVoxelThresholdValue = 0;
        maxVoxelThresholdValue = 65536;

        xDistance = 0.0f;
    }


    Renderer* createRenderer() const override 
    {
        qDebug() << "GLViewYZ";
        return new GLViewYZRenderer(Qt::red, (m_context ? m_context->getContext() : nullptr));
    }

    float minVoxelThreshold() const { return static_cast<float>(minVoxelThresholdValue) / 65536.0f; }
    float maxVoxelThreshold() const { return static_cast<float>(maxVoxelThresholdValue) / 65536.0f; }
    float getXDistance() const { return xDistance; }

    void setMinVoxelThreshold(int val)
    {
        if (minVoxelThresholdValue == val) return;
        minVoxelThresholdValue = val;
         update();
    }

    void setMaxVoxelThreshold(int val)
    {
        if (maxVoxelThresholdValue == val) return;
        maxVoxelThresholdValue = val;
         update();
    }

    ContextWrapper* context() const { return m_context; }

    void setContext(ContextWrapper* ctx)
    {
        if (m_context == ctx) return;
        m_context = ctx;

        connect(m_context, &ContextWrapper::volumeUpdated, this, &GLViewYZ::reloadData);

        emit contextChanged();
    }

    bool isRealodDataSet() const
    {
        return reloadDataFlag;
    }

    void reloadDataReset()
    {
        reloadDataFlag = false;
    }

signals:
     void contextChanged();

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
        xDistance += deltaY * 0.00005f;
        xDistance = __max(0.0, __min(1.0f, xDistance));
        m_context->getContext()->setXDistance(xDistance);
        update();
    }

private:
    void reloadData()
    {
        qDebug() << "initialize volume called from glviewyz.h";
        //reloadDataFlag = true;
    }

    float minVoxelThresholdValue;
    float maxVoxelThresholdValue;
    float xDistance;
    bool  reloadDataFlag = false;

    //context
    ContextWrapper* m_context = nullptr;
};