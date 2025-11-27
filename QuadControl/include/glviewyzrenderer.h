#pragma once
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <QColor>
#include <GLView.h>

class GLViewYZRenderer : public GLView
{
public:
    explicit GLViewYZRenderer(const QColor& color, std::shared_ptr<Context> c);
    ~GLViewYZRenderer() noexcept override = default;

protected:
    void initializeGL();
    void render() override;
    void synchronize(QQuickFramebufferObject* item) override;

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
