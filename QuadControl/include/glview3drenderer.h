#pragma once
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <QColor>
#include <GLView.h>
#include <QEvent>
#include <axes3d.h>
#include <axisPlane.h>
#include <camera.h>
#include <volume3dview.h>

class GLView3DRenderer : public GLView
{
public:
    explicit GLView3DRenderer(const QColor& color, std::shared_ptr<Context> c);
    ~GLView3DRenderer() noexcept override = default;

protected:
    void initializeGL();
    void render() override;
    void synchronize(QQuickFramebufferObject* item) override;

private:

    Camera camera;
    Axes3d axes3d;
    AxisPlane planeXY;
    AxisPlane planeXZ;
    AxisPlane planeYZ;
    Volume3dView volume3dview;

    int windowWidth;
    int windowHeight;

    //view parameters

    int minVoxelThresholdValue;
    int maxVoxelThresholdValue;

    //controls
    float rotateX;
    float rotateY;
    float cameraBoundaries;
};
