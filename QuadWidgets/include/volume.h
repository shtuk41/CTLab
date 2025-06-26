#pragma once

#include <vector>

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <glm/glm.hpp>
#include <camera.h>
#include <render_object.h>

class Volume : public RenderObject, public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
private:

    GLuint tex3D;
   	GLuint vertex_array_id = -1;
    GLuint vertex_buffer;
    GLuint invModelViewProj;
    GLuint cameraPos;
    GLuint aPos_attribute = -1;
    GLuint aTexCoord_attribute = -1;
         
    int xLength;
    int yLength;
    int zLength;

    Camera* cam;
    
public:

    Volume(int, int, int, Camera *);
    ~Volume();
    
    void SetProjection(glm::mat4 p);
    virtual void UpdateModel(const glm::mat4& cam_view);
    virtual void Setup();
    virtual void Draw();
};


