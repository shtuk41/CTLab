#pragma once
#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <QColor>
#include <Camera.h>

class GLView : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
    Q_OBJECT
public:
    explicit GLView(QWidget* parent = nullptr);
    ~GLView();

protected:
    //void initializeGL() override;
    //void resizeGL(int w, int h) override;
    //void paintGL() override;
    //Camera camera;

private:
    QOpenGLShaderProgram* shaderProgram = nullptr;
    GLuint vao = 0;
    GLuint vbo = 0;
};
