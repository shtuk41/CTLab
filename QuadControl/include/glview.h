#pragma once
#include <QOpenGLFramebufferObject>
#include <QQuickFramebufferObject>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions_3_3_Core>
#include <QColor>
#include <camera.h>
#include <context.h>
#include <viewBorder.h>

class GLView : public QQuickFramebufferObject::Renderer, protected QOpenGLFunctions_3_3_Core
{
public:
    explicit GLView(std::shared_ptr<Context> c, const QColor& color);
    ~GLView() noexcept = default;

    QOpenGLFramebufferObject* createFramebufferObject(const QSize& size) override {
        return new QOpenGLFramebufferObject(size, QOpenGLFramebufferObject::CombinedDepthStencil);
    }

protected:
    Camera camera;
    std::shared_ptr<Context> context;
    ViewBorder border;
    bool initialized;

private:
    QOpenGLShaderProgram* shaderProgram = nullptr;
    GLuint vao = 0;
    GLuint vbo = 0;
};
