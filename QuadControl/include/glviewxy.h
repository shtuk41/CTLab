#pragma once
#include <QQuickFramebufferObject>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QHoverEvent>

#include <algorithm>
#include <glm/glm.hpp>
#include <contextWrapper.h>
#include <glviewxyrenderer.h>

class GLViewXY : public QQuickFramebufferObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(int minVoxelThreshold READ minVoxelThreshold WRITE setMinVoxelThreshold)
    Q_PROPERTY(int maxVoxelThreshold READ maxVoxelThreshold WRITE setMaxVoxelThreshold)
    Q_PROPERTY(ContextWrapper* context READ context WRITE setContext NOTIFY contextChanged)

public:
    GLViewXY()
    {
        setAcceptedMouseButtons(Qt::AllButtons);  // enable mouse buttons
        setAcceptHoverEvents(true);               // enable hover events
        setFocusPolicy(Qt::StrongFocus);          // optional, for keyboard

        minVoxelThresholdValue = 0;
        maxVoxelThresholdValue = 65536;

        zDistance = 0.0f;
    }

    Renderer* createRenderer() const override 
    {
        qDebug() << "GLViewXY";
        return new GLViewXYRenderer(Qt::blue, (m_context ? m_context->getContext() : nullptr));
    }

    float minVoxelThreshold() const { return static_cast<float>(minVoxelThresholdValue) / 65536.0f; }
    float maxVoxelThreshold() const { return static_cast<float>(maxVoxelThresholdValue) / 65536.0f; }
    float getZDistance() const { return zDistance; }

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

        connect(m_context, &ContextWrapper::volumeUpdated, this, &GLViewXY::reloadData);

        emit contextChanged();
    }

    bool isRealodDataSet()
    {
        //qDebug() << "xy isRealodDataSet";
        return reloadDataFlag;
    }

    void reloadDataReset()
    {
        reloadDataFlag = false;
        update();
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
        int deltaZ = event->angleDelta().y();
        zDistance += deltaZ * 0.00005f;
        zDistance = std::max(0.0f, std::min(1.0f, zDistance));
        m_context->getContext()->setZDistance(zDistance);
        update();
    }
private:
    void reloadData()
    {
        reloadDataFlag = true;
        zDistance = 0.0f;
        update();
    }

    float minVoxelThresholdValue;
    float maxVoxelThresholdValue;
    float zDistance;
    bool  reloadDataFlag = false;
    
    //context
    ContextWrapper* m_context = nullptr;
};
