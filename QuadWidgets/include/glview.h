#pragma once
#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <QColor>
#include <camera.h>
#include <context.h>

class GLView : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
    Q_OBJECT
public:
    explicit GLView(QWidget* parent, Context*c);
    ~GLView();

protected:
    Camera camera;
    Context* context;

private:
    QOpenGLShaderProgram* shaderProgram = nullptr;
    GLuint vao = 0;
    GLuint vbo = 0;
};
