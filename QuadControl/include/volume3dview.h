#pragma once

#include <vector>

#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <glm/glm.hpp>
#include <camera.h>
#include <context.h>
#include <render_object.h>
#include <io/ioData.h>


class Volume3dView : public RenderObject, protected QOpenGLFunctions_3_3_Core
{
private:

    GLuint tex3D;
   	GLuint vertex_array_id = -1;
    GLuint vertex_buffer;
    GLuint invModelViewProj;
    GLuint cameraPos;
    GLuint aPos_attribute = -1;
    GLuint aTexCoord_attribute = -1;
    GLuint windowWidth;
    GLuint windowHeight;
    GLuint minVal;
    GLuint maxVal;
    GLuint cubeWorld;
    glm::vec3 cubeWorldVec;
         
    int windowWidthValue;
    int windowHeightValue;
    float minVoxelThresholdValue;
    float maxVoxelThresholdValue;

    Camera* cam;

    Context* context;
public:

    Volume3dView(Camera *cam);
    ~Volume3dView();
    
    void SetProjection(glm::mat4 p);
    void UpdateModel(const glm::mat4& cam_view,
        int winWidth,
        int winHeight,
        float minVoxelThreshold,
        float maxVoxelThreshold);
    virtual void UpdateModel(const glm::mat4& cam_view);
    virtual void Setup();
    virtual void Draw();

    void Setup(Context *c);
};


