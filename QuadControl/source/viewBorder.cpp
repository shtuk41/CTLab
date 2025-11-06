#include <qcolor.h>
#include <shaders.h>
#include <viewBorder.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <QOpenGLFramebufferObject>

ViewBorder::ViewBorder(float w, const QColor& c) : 
    width(w), 
    color(c.redF(), 
    c.greenF(), 
    c.blueF(), 
    c.alphaF())
{
}

ViewBorder::~ViewBorder()
{
    if (shaderProgram) delete shaderProgram;
    if (vertex_buffer[0]) glDeleteBuffers(2, vertex_buffer);
    if (vertex_array_id[0]) 
    {
        glDeleteVertexArrays(1, vertex_array_id);
        vertex_array_id[0] = 0;
    }
}

void ViewBorder::Setup()
{
    initializeOpenGLFunctions();

    shaderProgram = new QOpenGLShaderProgram();
    shaderProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, ".\\shaders\\viewBorder.vert");
    shaderProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, ".\\shaders\\viewBorder.frag");
    shaderProgram->link();

    glGenVertexArrays(1, vertex_array_id);
    glBindVertexArray(vertex_array_id[0]);

    std::vector<GLfloat> border = {
        // left
        -1,-1,0, -1 + width,-1,0, -1 + width,1,0, -1 + width,1,0, -1,1,0, -1,-1,0,
        // bottom
        -1,-1,0, 1,-1,0, 1,-1 + width,0, 1,-1 + width,0, -1,-1 + width,0, -1,-1,0,
        // right
        1,-1,0, 1,1,0, 1 - width,1,0, 1 - width,1,0, 1 - width,-1,0, 1,-1,0,
        // top
        -1,1,0, -1,1 - width,0, 1,1 - width,0, 1,1 - width,0, 1,1,0, -1,1,0
    };

    std::vector<GLfloat> borderColor(24 * 4);
    for (int i = 0; i < 24; i++) {
        borderColor[i * 4 + 0] = color.r;
        borderColor[i * 4 + 1] = color.g;
        borderColor[i * 4 + 2] = color.b;
        borderColor[i * 4 + 3] = color.a;
    }

    glGenBuffers(2, vertex_buffer);

    // Positions
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer[0]);
    glBufferData(GL_ARRAY_BUFFER, border.size() * sizeof(GLfloat), border.data(), GL_STATIC_DRAW);
    GLint posLoc = glGetAttribLocation(shaderProgram->programId(), "vPosition");
    glVertexAttribPointer(posLoc, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(posLoc);

    // Colors
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer[1]);
    glBufferData(GL_ARRAY_BUFFER, borderColor.size() * sizeof(GLfloat), borderColor.data(), GL_STATIC_DRAW);
    GLint colLoc = glGetAttribLocation(shaderProgram->programId(), "vColor");
    glVertexAttribPointer(colLoc, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(colLoc);

    glBindVertexArray(0);
}

void ViewBorder::Draw(const QOpenGLFramebufferObject* fbo)
{
    if (!fbo) return;

    glBindFramebuffer(GL_FRAMEBUFFER, fbo->handle());
    glViewport(0, 0, fbo->width(), fbo->height());

    shaderProgram->bind();
    glBindVertexArray(vertex_array_id[0]);
    glDrawArrays(GL_TRIANGLES, 0, 24);
    glBindVertexArray(0);
    shaderProgram->release();
}
