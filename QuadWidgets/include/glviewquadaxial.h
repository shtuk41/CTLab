#pragma once
#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <QColor>
#include <GLView.h>

class GLViewQuadAxial : public GLView
{
    Q_OBJECT
public:
    explicit GLViewQuadAxial(const QColor& color, QWidget* parent, Context* c);
    ~GLViewQuadAxial();

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

private:
    QOpenGLShaderProgram* shaderProgram = nullptr;
    QColor baseColor;

    GLuint vertex_array_id;
    GLuint vertex_buffer;
    GLint position_attribute = -1;
    GLuint tex3D;

    GLuint windowWidth;
    GLuint windowHeight;

    int windowWidthValue;
    int windowHeightValue;
};
