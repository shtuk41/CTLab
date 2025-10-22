#pragma once
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <QColor>
#include <GLView.h>

class GLViewSagittalRenderer : public GLView
{
public:
    explicit GLViewSagittalRenderer(const QColor& color, Context* c);
    ~GLViewSagittalRenderer() noexcept override = default;

    void UpdateMinMaxVoxelValues(int min, int max);

protected:
    void initializeGL();
    void render() override;

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
