#include <glviewxz.h>
#include <shaders.h>
#include <QMouseEvent>
#include <io/ioData.h>

GLViewXZRenderer::GLViewXZRenderer(const QColor& color, std::shared_ptr<Context> c)
    : GLView(c, color), baseColor(color)
{
    initializeGL();
}

void GLViewXZRenderer::initializeGL()
{
    initializeOpenGLFunctions();

    std::string vertexShaderSource = readSourceFile(".\\shaders\\xz.vert");
    std::string fragmentShaderSource = readSourceFile(".\\shaders\\xz.frag");

    shaderProgram = new QOpenGLShaderProgram();
    bool success = shaderProgram->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource.c_str());
    success = shaderProgram->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource.c_str());
    success = shaderProgram->link();

    const int widthX = context->volumeData->getHeader()->recoX;
    const int height = context->volumeData->getHeader()->recoY;
    const int depthY = context->volumeData->getHeader()->recoZ;

    float scaleX, scaleY;

    scaleY = 1.0f;
    scaleX = float(widthX) / depthY;

    GLfloat planeVertices[] = {
       -scaleX, scaleY, 0.0f, 0,0,
        scaleX, scaleY, 0.0f, 1,0,
        scaleX, -scaleY, 0.0f, 1,1,
        scaleX, -scaleY, 0.0f, 1,1,
       -scaleX, -scaleY, 0.0f, 0,1,
       -scaleX, scaleY, 0.0f, 0,0
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

    ySlice = glGetUniformLocation(shaderProgram->programId(), "ySlice");
    minVal = glGetUniformLocation(shaderProgram->programId(), "minVal");
    maxVal = glGetUniformLocation(shaderProgram->programId(), "maxVal");

    shaderProgram->bind();
    // Set the uniform sampler to use texture unit 1
    GLuint loc = glGetUniformLocation(shaderProgram->programId(), "volumeTex");
    glUniform1i(loc, 0);

    shaderProgram->release();

    border.Setup();
}

void GLViewXZRenderer::render()
{
    if (!context) return;

    if (!context->initialized) 
    {
        context->initGL();
    }

    auto fbo = framebufferObject();
    glBindFramebuffer(GL_FRAMEBUFFER, fbo->handle());
    glViewport(0, 0, fbo->width(), fbo->height());

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shaderProgram->bind();

    glUniform1f(minVal, minVoxelThresholdValue);
    glUniform1f(maxVal, maxVoxelThresholdValue);

    glUniform1f(ySlice, yDistance);

    // Activate texture unit 1 and bind your 3D texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_3D, context->tex3D);

    glBindVertexArray(vertex_array_id);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

    shaderProgram->release();

    border.Draw(fbo);
}

void GLViewXZRenderer::synchronize(QQuickFramebufferObject* item)
{
    auto* view = static_cast<GLViewXZ*>(item);

    //TODO:  verify that compiler inlines class members "simple getters"
    this->minVoxelThresholdValue = view->minVoxelThreshold();
    this->maxVoxelThresholdValue = view->maxVoxelThreshold();
    this->yDistance = view->getYDistance();
    glViewport(0, 0, view->width(), view->height());
}
