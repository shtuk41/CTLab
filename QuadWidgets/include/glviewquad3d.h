#pragma once
#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <QColor>
#include <GLView.h>
#include <QEvent>
#include <axes3d.h>
#include <camera.h>
#include <axisPlane.h>
#include <volume.h>

class GLViewQuad3D : public GLView
{
    Q_OBJECT
public:
    explicit GLViewQuad3D(const QColor& color, QWidget* parent = nullptr);
    ~GLViewQuad3D();

    void UpdateMinMaxVoxelValues(int min, int max);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void mouseMoveEvent(QMouseEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void enterEvent(QEnterEvent* event) override;
    void leaveEvent(QEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;

private:

    Camera camera;
    Axes3d axes3d;
    AxisPlane planeXY;
    AxisPlane planeXZ;
    AxisPlane planeYZ;
    Volume volume;

    int windowWidth;
    int windowHeight;

    //view parameters

    int minVoxelThresholdValue;
    int maxVoxelThresholdValue;

    //controls
    int previous_xpos;
    int previous_ypos;
    bool rotateEnable;
    bool moveback;
    bool moveforward;
    float rotateX;
    float rotateY;
    float cameraBoundaries;
};
