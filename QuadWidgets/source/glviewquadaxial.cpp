#include <glviewquadaxial.h>
#include <shaders.h>

GLViewQuadAxial::GLViewQuadAxial(const QColor& color, QWidget* parent, Context*c)
    : GLView(parent, c), baseColor(color)
{
}

GLViewQuadAxial::~GLViewQuadAxial()
{
    makeCurrent();
    glDeleteBuffers(1, &vertex_buffer);
    glDeleteVertexArrays(1, &vertex_array_id);
    delete shaderProgram;
    doneCurrent();
}

void GLViewQuadAxial::initializeGL()
{
    initializeOpenGLFunctions();

    std::string vertexShaderSource = readSourceFile(".\\shaders\\axialXY.vert");
    std::string fragmentShaderSource = readSourceFile(".\\shaders\\axialXY.frag");

    shaderProgram = new QOpenGLShaderProgram(this);
    bool success = shaderProgram->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource.c_str());
    success = shaderProgram->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource.c_str());
    success = shaderProgram->link();



    GLfloat planeVertices[] = {
       -1.0f,  -1.0f, 0.0f,  
       1.0f, -1.0f, 0.0f,  
       1.0f, 1.0f, 0.0f,
       1.0f, 1.0f, 0.0f,
       -1.0f, 1.0f, 0.0f,
       -1.0f, -1.0f, 0.0f,
    };

    glGenVertexArrays(1, &vertex_array_id);
    glGenBuffers(1, &vertex_buffer);

    glBindVertexArray(vertex_array_id);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
 

    const int width = context->volumeData.getHeader()->recoX;
    const int height = context->volumeData.getHeader()->recoY;
    const int depth = context->volumeData.getHeader()->recoZ;

    // === 2. Create 3D Texture ===

    glGenTextures(1, &tex3D);
    glBindTexture(GL_TEXTURE_3D, tex3D);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage3D(GL_TEXTURE_3D, 0, GL_R8, width, height, depth, 0,
        GL_RED, GL_UNSIGNED_BYTE, context->volumeData.getVolumeDataTex().data());

    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    windowWidth = glGetUniformLocation(shaderProgram->programId(), "windowWidth");
    windowHeight = glGetUniformLocation(shaderProgram->programId(), "windowHeight");
}

void GLViewQuadAxial::resizeGL(int w, int h)
{
    windowWidthValue = w;
    windowHeightValue = h;

    glViewport(0, 0, w, h);
}

void GLViewQuadAxial::paintGL()
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    shaderProgram->bind();
    QVector3D colorVec(baseColor.redF(), baseColor.greenF(), baseColor.blueF());
    shaderProgram->setUniformValue("baseColor", colorVec);

    glUniform1i(windowWidth, windowWidthValue);
    glUniform1i(windowHeight, windowHeightValue);

    // Activate texture unit 1 and bind your 3D texture
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_3D, tex3D);

    // Set the uniform sampler to use texture unit 1
    GLuint loc = glGetUniformLocation(shaderProgram->programId(), "volumeTex");
    glUniform1i(loc, 1);

    glBindVertexArray(vertex_array_id);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

    shaderProgram->release();
}
