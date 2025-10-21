#pragma once
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <QColor>
#include <GLView.h>


class GLViewAxialRenderer : public GLView
{
public:
    explicit GLViewAxialRenderer(const QColor& color, Context* c);
    ~GLViewAxialRenderer() noexcept override = default;

    void UpdateMinMaxVoxelValues(int min, int max);

protected:
    void initializeGL();
    void render() override;

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
