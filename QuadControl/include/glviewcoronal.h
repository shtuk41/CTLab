#pragma once
#include <QQuickFramebufferObject>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QHoverEvent>
#include <glm/glm.hpp>
#include <contextWrapper.h>
#include <glviewcoronalrenderer.h>

class GLViewCoronal : public QQuickFramebufferObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(int minVoxelThreshold READ minVoxelThreshold WRITE setMinVoxelThreshold NOTIFY minVoxelThresholdChanged)
    Q_PROPERTY(int maxVoxelThreshold READ maxVoxelThreshold WRITE setMaxVoxelThreshold NOTIFY maxVoxelThresholdChanged)
    Q_PROPERTY(ContextWrapper* context READ context WRITE setContext NOTIFY contextChanged)

public:
    GLViewCoronal()
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
        Context context(R"(D:\Files\Cesars\Scissors_Test 2025-7-2 15-11-21.uint16_scv)");
        context.volumeData.saveHeaderToFile("volumeHeader.txt");
        context.volumeData.fillBuffer();
        
        qDebug() << "GLViewCoronal";
        return new GLViewCoronalRenderer(Qt::green, &context);
    }

    float minVoxelThreshold() const { return static_cast<float>(minVoxelThresholdValue) / 65536.0f; }
    float maxVoxelThreshold() const { return static_cast<float>(maxVoxelThresholdValue) / 65536.0f; }
    float getXDistance() const { return xDistance; }

    void setMinVoxelThreshold(int val)
    {
        if (minVoxelThresholdValue == val) return;
        minVoxelThresholdValue = val;
        emit minVoxelThresholdChanged();
        update();
    }

    void setMaxVoxelThreshold(int val)
    {
        if (maxVoxelThresholdValue == val) return;
        maxVoxelThresholdValue = val;
        emit maxVoxelThresholdChanged();
        update();
    }

    ContextWrapper* context() const { return m_context; }

    void setContext(ContextWrapper* ctx)
    {
        if (m_context == ctx) return;
        m_context = ctx;
        emit contextChanged();
        initializeVolume();
    }

signals:
    void minVoxelThresholdChanged();
    void maxVoxelThresholdChanged();
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
        update();
    }

private:
    void initializeVolume()
    {
        if (!m_context) return;

        auto ctx = m_context->getContext();
        ctx->volumeData.saveHeaderToFile("volumeHeaderAxial.txt");
        ctx->volumeData.fillBuffer();
    }

    float minVoxelThresholdValue;
    float maxVoxelThresholdValue;
    float xDistance;

    //context
    ContextWrapper* m_context = nullptr;
};