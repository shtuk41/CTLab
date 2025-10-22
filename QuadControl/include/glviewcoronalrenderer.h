#pragma once
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <QColor>
#include <GLView.h>

class GLViewCoronalRenderer : public GLView
{
public:
    explicit GLViewCoronalRenderer(const QColor& color, Context* c);
    ~GLViewCoronalRenderer() noexcept override = default;
    void UpdateMinMaxVoxelValues(int min, int max);

protected:
    void initializeGL();
    void render() override;

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
