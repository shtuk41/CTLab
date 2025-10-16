#pragma once

#include <QQuickFramebufferObject>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions_3_3_Core>

class GLRenderer : public QQuickFramebufferObject::Renderer, protected QOpenGLFunctions_3_3_Core {
public:
    GLRenderer();
    void render() override;
    QOpenGLFramebufferObject* createFramebufferObject(const QSize& size) override;
private:
    QOpenGLShaderProgram program;
    GLuint vao = 0, vbo = 0;
};