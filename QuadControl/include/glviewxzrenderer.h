#pragma once
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <QColor>
#include <GLView.h>

class GLViewXZRenderer : public GLView
{
public:
    explicit GLViewXZRenderer(const QColor& color, std::shared_ptr<Context> c);
    ~GLViewXZRenderer() noexcept override = default;

protected:
    void initializeGL();
    void render() override;
    void synchronize(QQuickFramebufferObject* item) override;

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
