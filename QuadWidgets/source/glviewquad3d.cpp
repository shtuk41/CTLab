#include <glviewquad3d.h>
#include <shaders.h>
#include <QMouseEvent>

GLViewQuad3D::GLViewQuad3D(const QColor& color, QWidget* parent)
    : GLView(parent),
    axes3d(100, 100, 100),
    //Axis
    planeXY(glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec4(0.0f, 1.0f, 0.0f, 0.1f), 100),
    //Coronal
    planeXZ(glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec4(0.0f, 0.0f, 1.0f, 0.1f), 100),
    //Sagittal
    planeYZ(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 0.1f), 100),
    volume(256, 256, 256, &camera)
{
    setFocusPolicy(Qt::StrongFocus);

    previous_xpos = 0.0;
    previous_ypos = 0.0;
    rotateEnable = false;
    moveback = false;
    moveforward = false;
    rotateX = 0.0f;
    rotateY = 0.0f;
}

GLViewQuad3D::~GLViewQuad3D()
{
   
}

void GLViewQuad3D::initializeGL()
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

    volume.Setup();
}

void GLViewQuad3D::resizeGL(int w, int h)
{
    windowWidth = w;
    windowHeight = h;

    glViewport(0, 0, w, h);
}

void GLViewQuad3D::paintGL()
{
    glClearColor(0.9f, 0.9f, 0.9f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    camera.rotate2(rotateX, rotateY);

    camera.computeViewProjectionMatrices(-200, 200, -200, 200, -1500, 1500);

    glm::mat4 projection_matrix;
    glm::mat4 view_matrix;

    projection_matrix = camera.getProjectionMatrix();
    view_matrix = camera.getViewMatrix();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST);

    volume.UpdateModel(view_matrix, windowWidth, windowHeight);
    volume.SetProjection(projection_matrix);
    volume.Draw();

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

    
}

// Mouse click changes color to red
void GLViewQuad3D::mouseMoveEvent(QMouseEvent* event)
{
    //GLView::mouseMoveEvent(event);

    if (rotateEnable)
    {
        QPoint mouse_pos = event->pos();

        int w = width();
        int h = height();

        int delta_x = mouse_pos.x() - previous_xpos;
        int delta_y = mouse_pos.y() - previous_ypos;

        // Normalize mouse movement to [-1,1] relative to window size
        float norm_dx = float(delta_x) / float(w);
        float norm_dy = float(delta_y) / float(h);

        // Sensitivity factor for rotation speed
        float sensitivity = 3.0f;  

        float ry = sensitivity * norm_dx * glm::pi<float>();  // rotate around Y
        float rx = sensitivity * norm_dy * glm::pi<float>();  // rotate around X

        rotateY += ry;
        rotateX += rx;

        //this is important
        float maxPitch = glm::radians(89.0f);
        rotateX = glm::clamp(rotateX, -maxPitch, maxPitch);

        previous_xpos = mouse_pos.x();
        previous_ypos = mouse_pos.y();

        update();
    }
}


void GLViewQuad3D::mousePressEvent(QMouseEvent* event) 
{
    //GLView::mousePressEvent(event);
    
    if (event->button() == Qt::LeftButton) 
    {
        previous_xpos = event->pos().x();
        previous_ypos = event->pos().y();

        rotateEnable = true;
    }
}

void GLViewQuad3D::mouseReleaseEvent(QMouseEvent* event)
{
    //GLView::mouseReleaseEvent(event);

    if (event->button() == Qt::LeftButton)
    {
        rotateEnable = false;
    }
}

// Pressing 'G' sets color to green
void GLViewQuad3D::keyPressEvent(QKeyEvent* event) 
{
    auto key = event->key();
    //GLView::keyPressEvent(event);
    
    if (key == Qt::Key_Space)
    {
        rotateX = 0;
        rotateY = 0;
    }
}

void GLViewQuad3D::enterEvent(QEnterEvent* event)
{
    Q_UNUSED(event);
    setFocus();
}

void GLViewQuad3D::leaveEvent(QEvent* event) 
{
    Q_UNUSED(event);
    clearFocus();
}