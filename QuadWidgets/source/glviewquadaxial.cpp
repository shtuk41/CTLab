#include <glviewquadaxial.h>
#include <shaders.h>

GLViewQuadAxial::GLViewQuadAxial(const QColor& color, QWidget* parent)
    : GLView(parent), baseColor(color)
{
}

GLViewQuadAxial::~GLViewQuadAxial()
{
    makeCurrent();
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
    delete shaderProgram;
    doneCurrent();
}

void GLViewQuadAxial::initializeGL()
{
    initializeOpenGLFunctions();

    std::string vertexShaderSource = readSourceFile(".\\shaders\\quadview.vert");
    std::string fragmentShaderSource = readSourceFile(".\\shaders\\quadview.frag");

    shaderProgram = new QOpenGLShaderProgram(this);
    bool success = shaderProgram->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource.c_str());
    success = shaderProgram->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource.c_str());
    success = shaderProgram->link();

    GLfloat vertices[] = {
        0.0f,  0.5f, 0.0f,  // top
       -0.5f, -0.5f, 0.0f,  // left
        0.5f, -0.5f, 0.0f   // right
    };

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void GLViewQuadAxial::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void GLViewQuadAxial::paintGL()
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    shaderProgram->bind();
    QVector3D colorVec(baseColor.redF(), baseColor.greenF(), baseColor.blueF());
    shaderProgram->setUniformValue("baseColor", colorVec);

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);

    shaderProgram->release();
}
