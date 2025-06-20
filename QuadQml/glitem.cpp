#include "glitem.h"
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLFramebufferObject>

GLRenderer::GLRenderer() {
    initializeOpenGLFunctions();

    static const char* vsrc = R"(#version 330
    in vec2 position;
    void main() {
        gl_Position = vec4(position, 0.0, 1.0);
    })";

    static const char* fsrc = R"(#version 330
    out vec4 fragColor;
    void main() {
        fragColor = vec4(0.2, 0.6, 0.9, 1.0);
    })";

    bool success;

    success = program.addShaderFromSourceCode(QOpenGLShader::Vertex, vsrc);
    success = program.addShaderFromSourceCode(QOpenGLShader::Fragment, fsrc);
    success = program.link();

    float vertices[] = { -1, -1, 1, -1, 0, 1 };

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    program.bind();
    int posLoc = program.attributeLocation("position");
    glEnableVertexAttribArray(posLoc);
    glVertexAttribPointer(posLoc, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
    program.release();

    glBindVertexArray(0);
}

QOpenGLFramebufferObject* GLRenderer::createFramebufferObject(const QSize& size) {
    return new QOpenGLFramebufferObject(size, QOpenGLFramebufferObject::CombinedDepthStencil);
}

void GLRenderer::render() {
    glViewport(0, 0, framebufferObject()->width(), framebufferObject()->height());
    glClearColor(0, 1, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    program.bind();
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
    program.release();



    update(); // schedule next frame
}

QQuickFramebufferObject::Renderer* GLItem::createRenderer() const {
    return new GLRenderer();
}
