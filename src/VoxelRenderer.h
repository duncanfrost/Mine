#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <map>
class VoxelRenderer
{

public:

    VoxelRenderer();

    void Load(GLuint programID, std::vector<int> blockIDs);
    void Unload();

    void Draw(glm::mat4 ProjectionMatrix, glm::mat4 ViewMatrix, int ID);

    void SetTranslation(GLfloat x, GLfloat y, GLfloat z);

    void BindArrays();

private:

    void GenUVS(int gridX, int gridY, glm::vec3 normal, std::vector<glm::vec2> &uvs);
    void GenSide(glm::vec3 normal, std::vector<glm::vec3> &vertices,
                 std::vector<glm::vec3> &normals);

    GLuint vertexbuffer;
    GLuint normalbuffer;
    GLuint elementbuffer;
    GLuint uvBuff2;

    GLuint DiffuseTextureID;
    GLuint DiffuseTexture;

    GLuint MatrixID;
    GLuint ViewMatrixID;
    GLuint ModelMatrixID;
    GLuint ModelView3x3MatrixID;

    std::vector<unsigned short> indices;
    std::map<int, GLuint> uvBuffers;
    glm::mat4 ModelMatrix;

};
