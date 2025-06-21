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
    explicit GLViewQuadCoronal(const QColor& color, QWidget* parent = nullptr);
    ~GLViewQuadCoronal();

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

private:
    QOpenGLShaderProgram* shaderProgram = nullptr;
    GLuint vao = 0;
    GLuint vbo = 0;
    QColor baseColor;
};
