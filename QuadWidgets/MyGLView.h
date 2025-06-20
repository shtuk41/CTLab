#pragma once

// MyGLView.h
#include <QOpenGLWidget>
#include <QOpenGLFunctions>

class MyGLView : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT
public:
    explicit MyGLView(const QColor& clearColor, QWidget* parent = nullptr)
        : QOpenGLWidget(parent), m_clearColor(clearColor) {}

protected:
    void initializeGL() override {
        initializeOpenGLFunctions();
    }
    void paintGL() override {
        glClearColor(m_clearColor.redF(), m_clearColor.greenF(), m_clearColor.blueF(), 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }

private:
    QColor m_clearColor;
};

