#pragma once

#include "quadlib_global.h"
#include <QOpenGLFramebufferObject>
#include <QQuickFramebufferObject>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions_3_3_Core>

class GLRenderer : public QQuickFramebufferObject::Renderer, protected QOpenGLFunctions_3_3_Core {
public:
    GLRenderer() : initialized(false) {}

    void render() override {
        if (!initialized) {
            initializeOpenGLFunctions();

            static const char* vsrc = R"(#version 330
                in vec2 position;
                void main() { gl_Position = vec4(position, 0.0, 1.0); })";

            static const char* fsrc = R"(#version 330
                out vec4 fragColor;
                void main() { fragColor = vec4(0.2,0.6,0.9,1.0); })";

            program.addShaderFromSourceCode(QOpenGLShader::Vertex, vsrc);
            program.addShaderFromSourceCode(QOpenGLShader::Fragment, fsrc);
            program.link();

            float vertices[] = { -1, -1, 1, -1, 0, 1 };
            glGenVertexArrays(1, &vao);
            glGenBuffers(1, &vbo);
            glBindVertexArray(vao);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
            int posLoc = program.attributeLocation("position");
            glEnableVertexAttribArray(posLoc);
            glVertexAttribPointer(posLoc, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
            glBindVertexArray(0);

            initialized = true;
        }

        glViewport(0, 0, framebufferObject()->width(), framebufferObject()->height());
        glClearColor(0, 1, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        program.bind();
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);
        program.release();

        update();
    }

    QOpenGLFramebufferObject* createFramebufferObject(const QSize& size) override {
        return new QOpenGLFramebufferObject(size, QOpenGLFramebufferObject::CombinedDepthStencil);
    }

private:
    QOpenGLShaderProgram program;
    GLuint vao = 0, vbo = 0;
    bool initialized;
};
