#include <algorithm>
#include <shaders.h>
#include <volume3dview.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Volume3dView::Volume3dView(Camera* c) : cam(c)
{
}

Volume3dView::~Volume3dView()
{
    glDeleteBuffers(1, &vertex_buffer);
    glDeleteVertexArrays(1, &vertex_array_id);
    glDisableVertexAttribArray(aPos_attribute);
    glDisableVertexAttribArray(aTexCoord_attribute);
    delete shaderProgram;
}

void Volume3dView::Setup(std::shared_ptr<Context> ctx)
{
    context = ctx;

    Setup();
}

void Volume3dView::Setup()
{
    initializeOpenGLFunctions();

    std::string vertexShaderSource = readSourceFile(".\\shaders\\volume3dview.vert");
    std::string fragmentShaderSource = readSourceFile(".\\shaders\\volume3dview.frag");

    shaderProgram = new QOpenGLShaderProgram();
    bool success = shaderProgram->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource.c_str());
    success = shaderProgram->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource.c_str());
    success = shaderProgram->link();

    // === 3. Create Cube Geometry ===
    float cube[] = {
        // pos              // texcoord
        //-1,-1,-1,  0,0,0,
        // 1,-1,-1,  1,0,0,
        // 1, 1,-1,  1,1,0,
        //-1, 1,-1,  0,1,0,
        //-1,-1, 1,  0,0,1,
        // 1,-1, 1,  1,0,1,
        // 1, 1, 1,  1,1,1,
        //-1, 1, 1,  0,1,1,

        // Each face of the cube as triangles (12 triangles, 36 vertices total)
        // Back face
        -1,-1,-1,0,0,0,  1, 1,-1,1,1,0,  1,-1,-1,1,0,0,
        -1,-1,-1,0,0,0, -1, 1,-1,0,1,0,  1, 1,-1,1,1,0,
        // Front face
        -1,-1, 1,0,0,1,  1,-1, 1,1,0,1,  1, 1, 1,1,1,1,
        -1,-1, 1,0,0,1,  1, 1, 1,1,1,1, -1, 1, 1,0,1,1,
        // Left face
        -1,-1,-1,0,0,0, -1,-1, 1,0,0,1, -1, 1, 1,0,1,1,
        -1,-1,-1,0,0,0, -1, 1, 1,0,1,1, -1, 1,-1,0,1,0,
        // Right face
         1,-1,-1,1,0,0,  1, 1,-1,1,1,0,  1, 1, 1,1,1,1,
         1,-1,-1,1,0,0,  1, 1, 1,1,1,1,  1,-1, 1,1,0,1,
         // Bottom face
         -1,-1,-1,0,0,0,  1,-1,-1,1,0,0,  1,-1, 1,1,0,1,
         -1,-1,-1,0,0,0,  1,-1, 1,1,0,1, -1,-1, 1,0,0,1,
         // Top face
         -1, 1,-1,0,1,0, -1, 1, 1,0,1,1,  1, 1, 1,1,1,1,
         -1, 1,-1,0,1,0,  1, 1, 1,1,1,1,  1, 1,-1,1,1,0
    };

    int dims[3] = { context->width, context->height, context->depth };

    int maxDimElement = *std::max_element(dims, dims + 3);
    size_t dispSize = 50;

    for (int ii = 0; ii < 36; ++ii) 
    {
        cube[ii * 6 + 0] *= (dispSize * context->width / maxDimElement);   // x
        cube[ii * 6 + 1] *= (dispSize * context->height / maxDimElement);  // y
        cube[ii * 6 + 2] *= (dispSize * context->depth / maxDimElement);   // z
    }

    cubeWorldVec = { 2.0 * (100.0 * (static_cast<float>(context->width) / maxDimElement)),
                    2.0 * (100.0 * (static_cast<float>(context->height) / maxDimElement)),
                    2.0 * (100.0 * (static_cast<float>(context->depth) / maxDimElement)) };

    glGenVertexArrays(1, &vertex_array_id);
    glBindVertexArray(vertex_array_id);

    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);

    aPos_attribute = glGetAttribLocation(shaderProgram->programId(), "aPos");
    glVertexAttribPointer(aPos_attribute, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(aPos_attribute);

    aTexCoord_attribute = glGetAttribLocation(shaderProgram->programId(), "aTexCoord");
    glVertexAttribPointer(aTexCoord_attribute, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(aTexCoord_attribute);

    model_view = glGetUniformLocation(shaderProgram->programId(), "model_view");
    projection = glGetUniformLocation(shaderProgram->programId(), "projection");
    invModelViewProj = glGetUniformLocation(shaderProgram->programId(), "invMVP");
    cameraPos = glGetUniformLocation(shaderProgram->programId(), "cameraPos");
    windowWidth = glGetUniformLocation(shaderProgram->programId(), "windowWidth");
    windowHeight = glGetUniformLocation(shaderProgram->programId(), "windowHeight");
    minVal = glGetUniformLocation(shaderProgram->programId(), "minVal");
    maxVal = glGetUniformLocation(shaderProgram->programId(), "maxVal");
    cubeWorld = glGetUniformLocation(shaderProgram->programId(), "cubeWorld");

    model_matrix = glm::mat4(1.0f);
}

void Volume3dView::UpdateModel(const glm::mat4& cam_view, int winWidth, int winHeight, float minVoxelThreshold, float maxVoxelThreshold)
{
    UpdateModel(cam_view);

    windowWidthValue = winWidth;
    windowHeightValue = winHeight;
    minVoxelThresholdValue = minVoxelThreshold;
    maxVoxelThresholdValue = maxVoxelThreshold;
}

void Volume3dView::UpdateModel(const glm::mat4& cam_view)
{
    SetPosition(0, 0, 0);

    auto mm = glm::translate(model_matrix, glm::vec3(_X, _Y, _Z));
    model_view_matrix = cam_view * mm;
}

void Volume3dView::SetProjection(glm::mat4 p)
{
    projection_matrix = p;
}

void Volume3dView::Draw()
{
    if (!context) return;

    if (!context->initialized)
    {
        context->initGL();
    }

    shaderProgram->bind();

    glUniformMatrix4fv(model_view, 1, GL_FALSE, glm::value_ptr(model_view_matrix));
    glUniformMatrix4fv(projection, 1, GL_FALSE, glm::value_ptr(projection_matrix));

    glm::mat4 mvp = projection_matrix * model_view_matrix;
    glm::mat4 invMVP = glm::inverse(mvp);

    glUniformMatrix4fv(invModelViewProj, 1, GL_FALSE, glm::value_ptr(invMVP));

    glm::vec3 camPosition = cam->getPosition();
    glUniform3fv(cameraPos, 1, glm::value_ptr(camPosition));

    glUniform1i(windowWidth, windowWidthValue);
    glUniform1i(windowHeight, windowHeightValue);

    glUniform1f(minVal, minVoxelThresholdValue);
    glUniform1f(maxVal, maxVoxelThresholdValue);

    glUniform3fv(cubeWorld, 1, glm::value_ptr(cubeWorldVec));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_3D, context->tex3D);
    GLuint te = glGetUniformLocation(shaderProgram->programId(), "volumeTex");
    glUniform1i(te, 0);

    glBindVertexArray(vertex_array_id);
    glDrawArrays(GL_TRIANGLES, 0, 36);  // full cube

    shaderProgram->release();
}
