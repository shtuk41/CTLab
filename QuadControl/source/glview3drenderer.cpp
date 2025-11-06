#include <glview3d.h>
#include <shaders.h>
#include <QMouseEvent>
#include <io/ioData.h>

GLView3DRenderer::GLView3DRenderer(const QColor& color, std::shared_ptr<Context> c)
    : GLView(c, color),
    axes3d(100, 100, 100),
    //Axis
    planeXY(glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec4(0.0f, 1.0f, 0.0f, 0.1f), 100),
    //Coronal
    planeXZ(glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec4(0.0f, 0.0f, 1.0f, 0.1f), 100),
    //Sagittal
    planeYZ(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 0.1f), 100),
    volume3dview(&camera),
    cameraBoundaries(200)
{
    initializeGL();
}

void GLView3DRenderer::initializeGL()
{
    initializeOpenGLFunctions();

    //glEnable(GL_DEPTH_TEST);

    axes3d.Setup();

    //Axis
    planeXY.Setup();
    //Coronal
    planeXZ.Setup();
    //Sagittal
    planeYZ.Setup();
    //3d view
    volume3dview.Setup(context);

    border.Setup();
}

void GLView3DRenderer::render()
{
    auto fbo = framebufferObject();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    camera.rotate2(rotateX, rotateY);

    camera.computeViewProjectionMatrices(-cameraBoundaries, cameraBoundaries, -cameraBoundaries, cameraBoundaries, -2000, 2000);

    glm::mat4 projection_matrix;
    glm::mat4 view_matrix;

    projection_matrix = camera.getProjectionMatrix();
    view_matrix = camera.getViewMatrix();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable (GL_DEPTH_TEST);

    volume3dview.UpdateModel(view_matrix, windowWidth, windowHeight, float(minVoxelThresholdValue) / 65535, float(maxVoxelThresholdValue)/ 65535);
    volume3dview.SetProjection(projection_matrix);
    volume3dview.Draw();

    glDisable(GL_DEPTH_TEST);

    axes3d.UpdateModel(view_matrix);
    axes3d.SetProjection(projection_matrix);
    axes3d.Draw();

    planeXY.UpdateModel(view_matrix);
    planeXY.SetProjection(projection_matrix);
    planeXY.Draw();
    planeXZ.UpdateModel(view_matrix);
    planeXZ.SetProjection(projection_matrix);
    planeXZ.Draw();
    planeYZ.UpdateModel(view_matrix);
    planeYZ.SetProjection(projection_matrix);
    planeYZ.Draw();

    border.Draw(fbo);
}

void GLView3DRenderer::synchronize(QQuickFramebufferObject* item)
{
    auto* view = static_cast<GLView3D*>(item);

    //TODO:  verify that compiler inlines class members "simple getters"
    this->rotateX = view->getRotateX();
    this->rotateY = view->getRotateY();
    this->cameraBoundaries = view->getCameraBoundaries();
    this->windowWidth = view->width();
    this->windowHeight = view->height();
    this->minVoxelThresholdValue = view->minVoxelThreshold();
    this->maxVoxelThresholdValue = view->maxVoxelThreshold();

}