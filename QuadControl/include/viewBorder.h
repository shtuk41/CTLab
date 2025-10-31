#pragma once

#include <memory>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <glm/glm.hpp>
#include <render_object.h>
#include <io/ioData.h>

class ViewBorder : protected QOpenGLFunctions_3_3_Core
{
private:
    QOpenGLShaderProgram* shaderProgram = nullptr;
   	GLuint vertex_array_id[1];
    GLuint vertex_buffer[2];
	GLint position_attribute = -1;
	GLint color_attribute = -1;

    glm::vec4 color;
    float width;

public:

    ViewBorder(float w, const QColor& color);
    ~ViewBorder();

    void Setup();
    void Draw(const QOpenGLFramebufferObject* fbo);
};


