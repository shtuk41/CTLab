#include "MyGLView.h"

static const char* vertexShaderSource = R"(
#version 330 core
layout(location = 0) in vec3 pos;
void main() {
    gl_Position = vec4(pos, 1.0);
}
)";

static const char* fragmentShaderSource = R"(
#version 330 core
uniform vec3 baseColor;
out vec4 fragColor;
void main() {
    fragColor = vec4(baseColor, 1.0);
}
)";

MyGLView::MyGLView(const QColor& color, QWidget* parent)
    : QOpenGLWidget(parent), baseColor(color)
{
}

MyGLView::~MyGLView()
{
    makeCurrent();
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
    delete shaderProgram;
    doneCurrent();
}

void MyGLView::initializeGL()
{
    initializeOpenGLFunctions();

    shaderProgram = new QOpenGLShaderProgram(this);
    shaderProgram->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
    shaderProgram->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
    shaderProgram->link();

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

void MyGLView::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void MyGLView::paintGL()
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
