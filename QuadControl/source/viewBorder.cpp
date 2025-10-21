#include <qcolor.h>
#include <shaders.h>
#include <viewBorder.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

ViewBorder::ViewBorder(float w, const QColor& c) : width(w), 
                                                color(c.redF(), c.greenF(), c.blueF(), c.alphaF())
{
}

ViewBorder::~ViewBorder()
{
    glDeleteBuffers(2, vertex_buffer);
    glDeleteVertexArrays(1, vertex_array_id);
    glDisableVertexAttribArray(position_attribute);
    glDisableVertexAttribArray(color_attribute);
    delete shaderProgram;
}

void ViewBorder::Setup()
{
    initializeOpenGLFunctions();

    std::string vertexShaderSource = readSourceFile(".\\shaders\\viewBorder.vert");
    std::string fragmentShaderSource = readSourceFile(".\\shaders\\viewBorder.frag");

    shaderProgram = new QOpenGLShaderProgram();
    bool success = shaderProgram->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource.c_str());
    success = shaderProgram->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource.c_str());
    success = shaderProgram->link();

    glGenVertexArrays(1, vertex_array_id);
    glBindVertexArray(vertex_array_id[0]);

    

    GLfloat border[] = {
            -1, -1, 0,
            -1 + width, -1, 0,
            -1 + width, 1, 0,
            -1 + width, 1, 0,
            -1, 1, 0,
            -1, -1, 0,

            -1, -1, 0,
            1, -1, 0,
            1, -1 + width, 0,
            1, -1 + width, 0,
            -1, -1 + width, 0,
            -1, -1, 0,

            1, -1, 0,
            1, 1, 0,
            1 - width, 1, 0,
            1 - width, 1, 0,
            1 - width, -1, 0,
            1, -1, 0,

            -1, 1, 0,
            -1, 1 - width, 0,
            1, 1 - width, 0,
            1, 1 - width, 0,
            1, 1, 0,
            -1, 1, 0
    };

    GLfloat borderColor[] =
    {
        color.r,color.g, color.b, 1.0,
        color.r,color.g, color.b, 1.0,
        color.r,color.g, color.b, 1.0,
        color.r,color.g, color.b, 1.0,
        color.r,color.g, color.b, 1.0,
        color.r,color.g, color.b, 1.0,
        color.r,color.g, color.b, 1.0,
        color.r,color.g, color.b, 1.0,
        color.r,color.g, color.b, 1.0,
        color.r,color.g, color.b, 1.0,
        color.r,color.g, color.b, 1.0,
        color.r,color.g, color.b, 1.0,
        color.r,color.g, color.b, 1.0,
        color.r,color.g, color.b, 1.0,
        color.r,color.g, color.b, 1.0,
        color.r,color.g, color.b, 1.0,
        color.r,color.g, color.b, 1.0,
        color.r,color.g, color.b, 1.0,
        color.r,color.g, color.b, 1.0,
        color.r,color.g, color.b, 1.0,
        color.r,color.g, color.b, 1.0,
        color.r,color.g, color.b, 1.0,
        color.r,color.g, color.b, 1.0,
        color.r,color.g, color.b, 1.0,
    };

    glGenBuffers(2, vertex_buffer);

    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(border), border, GL_STATIC_DRAW);
    position_attribute = glGetAttribLocation(shaderProgram->programId(), "vPosition");
    glVertexAttribPointer(position_attribute, 3, GL_FLOAT, GL_FALSE, 0, (void*)nullptr);
    glEnableVertexAttribArray(position_attribute);

    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(borderColor), borderColor, GL_STATIC_DRAW);
    color_attribute = glGetAttribLocation(shaderProgram->programId(), "vColor");
    glVertexAttribPointer(color_attribute, 4, GL_FLOAT, GL_FALSE, 0, (void*)nullptr);
    glEnableVertexAttribArray(color_attribute);
}

void ViewBorder::Draw()
{
    shaderProgram->bind();

    glBindVertexArray(vertex_array_id[0]);
    glDrawArrays(GL_TRIANGLES, 0, 24);

    shaderProgram->release();
}
