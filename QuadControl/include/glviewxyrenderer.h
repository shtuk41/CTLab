#pragma once
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <QColor>
#include <glview.h>


class GLViewXYRenderer : public GLView
{
public:
    explicit GLViewXYRenderer(const QColor& color, std::shared_ptr<Context> c);
    ~GLViewXYRenderer() noexcept override = default;

protected:
    void initializeGL();
    void render() override;
    void synchronize(QQuickFramebufferObject* item) override;
    void reloadData();
    void deleteBuffers();

private:
    QOpenGLShaderProgram* shaderProgram;
    QColor baseColor;

    GLuint vertex_array_id;
    GLuint vertex_buffer;
    GLuint tex3D;
    GLuint zSlice;
    GLuint minVal;
    GLuint maxVal;

    float minVoxelThresholdValue;
    float maxVoxelThresholdValue;
    float zDistance;
};
