#include <glviewquadxy.h>
#include <shaders.h>

GLViewQuadXY::GLViewQuadXY(const QColor& color, QWidget* parent, Context*c)
    : GLView(parent, c, color), baseColor(color)
{
}

GLViewQuadXY::~GLViewQuadXY()
{
    makeCurrent();
    deleteBuffers();
    delete shaderProgram;
    doneCurrent();
}

void GLViewQuadXY::updateVolume()
{
    makeCurrent();    
    reloadData();     
    doneCurrent();    
    update();         
}

void GLViewQuadXY::deleteBuffers()
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

void GLViewQuadXY::reloadData()
{
    deleteBuffers();

    zDistance = 0;

    const int widthX = context->volumeData->getHeader()->recoX;
    const int heightY = context->volumeData->getHeader()->recoY;
    const int depthZ = context->volumeData->getHeader()->recoZ;

    float scaleX, scaleY;

    scaleY = 1.0f;
    scaleX = float(widthX) / heightY;

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
    glTexImage3D(GL_TEXTURE_3D, 0, GL_R8, widthX, heightY, depthZ, 0,
        GL_RED, GL_UNSIGNED_BYTE, context->volumeData->getVolumeDataTex().data());

    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    zSlice = glGetUniformLocation(shaderProgram->programId(), "zSlice");
    minVal = glGetUniformLocation(shaderProgram->programId(), "minVal");
    maxVal = glGetUniformLocation(shaderProgram->programId(), "maxVal");

    //shaderProgram->bind();
    // Set the uniform sampler to use texture unit 1
    GLuint loc = glGetUniformLocation(shaderProgram->programId(), "volumeTex");
    glUniform1i(loc, 0);

    //shaderProgram->release();
    update();
}

void GLViewQuadXY::initializeGL()
{
    initializeOpenGLFunctions();

    std::string vertexShaderSource = readSourceFile(".\\shaders\\xy.vert");
    std::string fragmentShaderSource = readSourceFile(".\\shaders\\xy.frag");

    shaderProgram = new QOpenGLShaderProgram(this);
    bool success = shaderProgram->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource.c_str());
    success = shaderProgram->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource.c_str());
    success = shaderProgram->link();

    reloadData();

    border.Setup();
}

void GLViewQuadXY::resizeGL(int w, int h)
{
    float scaleX, scaleY;

    const float aspect = float(w) / h;

    const int widthX = context->volumeData->getHeader()->recoX;
    const int heightY = context->volumeData->getHeader()->recoY;

    scaleY = 1.0f;
    scaleX = float(widthX) / heightY;

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

void GLViewQuadXY::paintGL()
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    shaderProgram->bind();

    glUniform1f(minVal, getMinVoxelThresholdValue());
    glUniform1f(maxVal, getMaxVoxelThresholdValue());

    glUniform1f(zSlice, zDistance);

    // Activate texture unit 1 and bind your 3D texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_3D, tex3D);

    glBindVertexArray(vertex_array_id);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

    shaderProgram->release();

    border.Draw();
}

void GLViewQuadXY::mouseMoveEvent(QMouseEvent* event)
{
    Q_UNUSED(event);
    setFocus();
}

void GLViewQuadXY::mousePressEvent(QMouseEvent* event)
{
    Q_UNUSED(event);
    setFocus();
}

void GLViewQuadXY::mouseReleaseEvent(QMouseEvent* event)
{
    Q_UNUSED(event);
    setFocus();
}
void GLViewQuadXY::keyPressEvent(QKeyEvent* event)
{
    Q_UNUSED(event);
    setFocus();
}

void GLViewQuadXY::enterEvent(QEnterEvent* event)
{
    Q_UNUSED(event);
    setFocus();
}

void GLViewQuadXY::leaveEvent(QEvent* event)
{
    Q_UNUSED(event);
    setFocus();
}

void GLViewQuadXY::wheelEvent(QWheelEvent* event)
{
    int deltaZ = event->angleDelta().y();
    zDistance += deltaZ * 0.00005f;
    zDistance = __max(0.0, __min(1.0f, zDistance));
    context->setZDistance(zDistance);

    update();
}
