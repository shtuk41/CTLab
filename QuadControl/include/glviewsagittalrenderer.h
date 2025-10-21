#pragma once
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <QColor>
#include <GLView.h>

class GLViewSagittalRenderer : public GLView
{
public:
    explicit GLViewSagittalRenderer(const QColor& color, Context* c);
    ~GLViewSagittalRenderer();

    void UpdateMinMaxVoxelValues(int min, int max);
    void render() override;

protected:
    void initializeGL();
    void render() override;

    void mouseMoveEvent(QMouseEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void enterEvent(QEnterEvent* event) override;
    void leaveEvent(QEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;

private:
    QOpenGLShaderProgram* shaderProgram = nullptr;
    QColor baseColor;
    float minVoxelThresholdValue;
    float maxVoxelThresholdValue;
    float yDistance;

    GLuint vertex_array_id;
    GLuint vertex_buffer;
    GLuint tex3D;
    GLuint ySlice;
    GLuint minVal;
    GLuint maxVal;
};
