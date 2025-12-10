#include <glviewquadyz.h>
#include <shaders.h>

GLViewQuadYZ::GLViewQuadYZ(const QColor& color, QWidget* parent, Context *c)
    : GLView(parent, c, color), baseColor(color)
{
}

GLViewQuadYZ::~GLViewQuadYZ()
{
    makeCurrent();
    deleteBuffers();
    delete shaderProgram;
    doneCurrent();
}

void GLViewQuadYZ::updateVolume()
{
    makeCurrent();
    reloadData();
    doneCurrent();
    update();
}

void GLViewQuadYZ::deleteBuffers()
{
    if (tex3D)
    {
        glDeleteTextures(1, &tex3D);
        tex3D = 0;
    }

    if (vertex_buffer)
    {
        glDeleteBuffers(1, &vertex_buffer);
        vertex_buffer = 0;
    }

    if (vertex_array_id)
    {
        glDeleteVertexArrays(1, &vertex_array_id);
        vertex_array_id = 0;
    }
}

void GLViewQuadYZ::reloadData()
{
    deleteBuffers();

    const int width = context->volumeData->getHeader()->recoX;
    const int heightX = context->volumeData->getHeader()->recoY;
    const int depthY = context->volumeData->getHeader()->recoZ;

    float scaleX, scaleY;

    scaleY = 1.0f;
    scaleX = float(heightX) / depthY;

    GLfloat planeVertices[] = {
       -scaleX, -scaleY, 0.0f, 0,0,
        scaleX, -scaleY, 0.0f, 1,0,
        scaleX,  scaleY, 0.0f, 1,1,
        scaleX,  scaleY, 0.0f, 1,1,
       -scaleX,  scaleY, 0.0f, 0,1,
       -scaleX, -scaleY, 0.0f, 0,0
    };

    glGenVertexArrays(1, &vertex_array_id);
    glGenBuffers(1, &vertex_buffer);

    glBindVertexArray(vertex_array_id);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // === 2. Create 3D Texture ===

    glGenTextures(1, &tex3D);
    glBindTexture(GL_TEXTURE_3D, tex3D);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage3D(GL_TEXTURE_3D, 0, GL_R8, width, heightX, depthY, 0,
        GL_RED, GL_UNSIGNED_BYTE, context->volumeData->getVolumeDataTex().data());

    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    xSlice = glGetUniformLocation(shaderProgram->programId(), "xSlice");
    minVal = glGetUniformLocation(shaderProgram->programId(), "minVal");
    maxVal = glGetUniformLocation(shaderProgram->programId(), "maxVal");

    shaderProgram->bind();
    // Set the uniform sampler to use texture unit 1
    GLuint loc = glGetUniformLocation(shaderProgram->programId(), "volumeTex");
    glUniform1i(loc, 0);

    shaderProgram->release();

    update();
}

void GLViewQuadYZ::initializeGL()
{
    initializeOpenGLFunctions();

    std::string vertexShaderSource = readSourceFile(".\\shaders\\yz.vert");
    std::string fragmentShaderSource = readSourceFile(".\\shaders\\yz.frag");

    shaderProgram = new QOpenGLShaderProgram(this);
    bool success = shaderProgram->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource.c_str());
    success = shaderProgram->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource.c_str());
    success = shaderProgram->link();

    reloadData();

    border.Setup();
}

void GLViewQuadYZ::resizeGL(int w, int h)
{
    const int heightX = context->volumeData->getHeader()->recoY;
    const int depthY = context->volumeData->getHeader()->recoZ;

    float scaleX, scaleY;

    scaleY = 1.0f;              
    scaleX = float(heightX) / depthY;

    GLfloat planeVertices[] = {
       -scaleX, -scaleY, 0.0f, 0,0,
        scaleX, -scaleY, 0.0f, 1,0,
        scaleX,  scaleY, 0.0f, 1,1,
        scaleX,  scaleY, 0.0f, 1,1,
       -scaleX,  scaleY, 0.0f, 0,1,
       -scaleX, -scaleY, 0.0f, 0,0
    };

    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(planeVertices), planeVertices);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glViewport(0, 0, w, h);
}

void GLViewQuadYZ::paintGL()
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    shaderProgram->bind();

    glUniform1f(minVal, getMinVoxelThresholdValue());
    glUniform1f(maxVal, getMaxVoxelThresholdValue());

    glUniform1f(xSlice, xDistance);

    // Activate texture unit 1 and bind your 3D texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_3D, tex3D);

    glBindVertexArray(vertex_array_id);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

    shaderProgram->release();

    border.Draw();
}

void GLViewQuadYZ::mouseMoveEvent(QMouseEvent* event)
{
    Q_UNUSED(event);
    setFocus();
}

void GLViewQuadYZ::mousePressEvent(QMouseEvent* event)
{
    Q_UNUSED(event);
    setFocus();
}

void GLViewQuadYZ::mouseReleaseEvent(QMouseEvent* event)
{
    Q_UNUSED(event);
    setFocus();
}
void GLViewQuadYZ::keyPressEvent(QKeyEvent* event)
{
    Q_UNUSED(event);
    setFocus();
}

void GLViewQuadYZ::enterEvent(QEnterEvent* event)
{
    Q_UNUSED(event);
    setFocus();
}

void GLViewQuadYZ::leaveEvent(QEvent* event)
{
    Q_UNUSED(event);
    setFocus();
}

void GLViewQuadYZ::wheelEvent(QWheelEvent* event)
{
    int deltaX = event->angleDelta().y();
    xDistance += deltaX * 0.00005f;
    xDistance = __max(0.0, __min(1.0f, xDistance));
    context->setXDistance(xDistance);
    update();
}
