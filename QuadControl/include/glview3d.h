#pragma once
#include <QQmlEngine>
#include <QQmlContext>
#include <QQuickFramebufferObject>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QHoverEvent>
#include <glm/glm.hpp>
#include <contextWrapper.h>
#include <glView3dRenderer.h>

class GLView3D : public QQuickFramebufferObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(int minVoxelThreshold READ minVoxelThreshold WRITE setMinVoxelThreshold NOTIFY minVoxelThresholdChanged)
    Q_PROPERTY(int maxVoxelThreshold READ maxVoxelThreshold WRITE setMaxVoxelThreshold NOTIFY maxVoxelThresholdChanged)
    Q_PROPERTY(ContextWrapper* context READ context WRITE setContext NOTIFY contextChanged)

public:
    GLView3D()
    {
        setAcceptedMouseButtons(Qt::AllButtons);  // enable mouse buttons
        setAcceptHoverEvents(true);               // enable hover events
        setFocusPolicy(Qt::StrongFocus);          // optional, for keyboard

        minVoxelThresholdValue = 0;
        maxVoxelThresholdValue = 65536;

        previous_xpos = 0.0;
        previous_ypos = 0.0;
        rotateEnable = false;
        moveback = false;
        moveforward = false;
        rotateX = 0.0f;
        rotateY = 0.0f;
    }

    Renderer* createRenderer() const override 
    {
        qDebug() << "GLView3D";

        if (!m_context)
            qDebug() << "Context not created";

        return new GLView3DRenderer(Qt::yellow, (m_context ? m_context->getContext() : nullptr));
    }

    float getRotateX() const { return rotateX; };
    float getRotateY() const { return rotateY; };
    float getCameraBoundaries() const { return cameraBoundaries; };
    int minVoxelThreshold() const { return minVoxelThresholdValue; }
    int maxVoxelThreshold() const { return maxVoxelThresholdValue; }

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
         
        m_context->getContext()->onDistanceChanged = [this]() {
            this->update();
            };

        connect(m_context, &ContextWrapper::volumeUpdated, this, &GLView3D::reloadData);

        emit contextChanged();
    }

    bool isRealodDataSet() const
    {
        return reloadDataFlag;
    }

    void reloadDataReset()
    {
        reloadDataFlag = false;
        update();
    }

signals:
    void minVoxelThresholdChanged();
    void maxVoxelThresholdChanged();
    void contextChanged();

protected:


    void mouseMoveEvent(QMouseEvent* event) override
    {
        if (rotateEnable)
        {
            QPoint mouse_pos = event->pos();

            int w = width();
            int h = height();

            int delta_x = mouse_pos.x() - previous_xpos;
            int delta_y = mouse_pos.y() - previous_ypos;

            // Normalize mouse movement to [-1,1] relative to window size
            float norm_dx = float(delta_x) / float(w);
            float norm_dy = float(delta_y) / float(h);

            // Sensitivity factor for rotation speed
            float sensitivity = 3.0f;

            float ry = sensitivity * norm_dx * glm::pi<float>();  // rotate around Y
            float rx = sensitivity * norm_dy * glm::pi<float>();  // rotate around X

            rotateY += ry;
            rotateX += rx;

            //this is important
            float maxPitch = glm::radians(89.0f);
            rotateX = glm::clamp(rotateX, -maxPitch, maxPitch);

            previous_xpos = mouse_pos.x();
            previous_ypos = mouse_pos.y();

            update();
        }
    }

    void mousePressEvent(QMouseEvent* event) override
    {
        if (event->button() == Qt::LeftButton)
        {
            previous_xpos = event->pos().x();
            previous_ypos = event->pos().y();

            rotateEnable = true;
        }
    }

    void mouseReleaseEvent(QMouseEvent* event) override
    {
        if (event->button() == Qt::LeftButton)
        {
            rotateEnable = false;
        }
    }

    void keyPressEvent(QKeyEvent* event) override
    {
        auto key = event->key();
        //GLView::keyPressEvent(event);

        if (key == Qt::Key_Space)
        {
            rotateX = 0;
            rotateY = 0;
        }
    }

    void hoverEnterEvent(QHoverEvent* event) override
    {
        Q_UNUSED(event);
        //setFocus();
    }

    void hoverLeaveEvent(QHoverEvent* event) override
    {
        Q_UNUSED(event);
        //clearFocus();
    }

    void wheelEvent(QWheelEvent* event) override
    {
        int deltaY = event->angleDelta().y();
        cameraBoundaries += deltaY * 0.1;
        cameraBoundaries = __max(1, __min(2000, cameraBoundaries));
        update();
    }

private:
    void reloadData()
    {
        reloadDataFlag = true;
        previous_xpos = 0.0;
        previous_ypos = 0.0;
        rotateEnable = false;
        moveback = false;
        moveforward = false;
        rotateX = 0.0f;
        rotateY = 0.0f;
        cameraBoundaries = 1.0f;
        update();
    }

    int windowWidth;
    int windowHeight;

    //view parameters

    int minVoxelThresholdValue;
    int maxVoxelThresholdValue;

    //controls
    int previous_xpos;
    int previous_ypos;
    bool rotateEnable;
    bool moveback;
    bool moveforward;
    float rotateX;
    float rotateY;
    float cameraBoundaries = 1.0f;

    bool reloadDataFlag = false;

    //context
    ContextWrapper* m_context = nullptr;
};
