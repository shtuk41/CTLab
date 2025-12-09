#pragma once
#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <QColor>
#include <camera.h>
#include <context.h>
#include <viewBorder.h>

class GLView : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
    Q_OBJECT
public:
    explicit GLView(QWidget* parent, Context*c, const QColor& color);
    ~GLView();

    void UpdateMinMaxVoxelValues(int min, int max)
    {
        minVoxelThresholdValue = float(min) / 65535;
        maxVoxelThresholdValue = float(max) / 65535;

        update();
    }

    float getMinVoxelThresholdValue() const { return minVoxelThresholdValue; }
    float getMaxVoxelThresholdValue() const { return maxVoxelThresholdValue; }
 
    virtual void reloadData() = 0;
    virtual void deleteBuffers() = 0;

protected:
    Camera camera;
    Context* context;
    ViewBorder border;



private:
    QOpenGLShaderProgram* shaderProgram = nullptr;
    GLuint vao = 0;
    GLuint vbo = 0;
    float minVoxelThresholdValue;
    float maxVoxelThresholdValue;
};
