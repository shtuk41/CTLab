#pragma once
#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <QColor>
#include <GLView.h>

class GLViewQuadCoronal : public GLView
{
    Q_OBJECT
public:
    explicit GLViewQuadCoronal(const QColor& color, QWidget* parent, Context* c);
    ~GLViewQuadCoronal();
    void UpdateMinMaxVoxelValues(int min, int max);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

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
    float xDistance;

    GLuint vertex_array_id;
    GLuint vertex_buffer;
    GLuint tex3D;
    GLuint xSlice;
    GLuint minVal;
    GLuint maxVal;
};
