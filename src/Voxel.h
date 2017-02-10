#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>
class Voxel
{

public:

    Voxel();

    void Load(GLuint programID, int ID);
    void Unload();

    void Draw(glm::mat4 ProjectionMatrix, glm::mat4 ViewMatrix);

    void SetTranslation(GLfloat x, GLfloat y, GLfloat z);

    void BindArrays();

private:
    GLuint vertexbuffer;
    GLuint uvbuffer;
    GLuint normalbuffer;
    GLuint tangentbuffer;
    GLuint bitangentbuffer;
    GLuint elementbuffer;

    GLuint DiffuseTextureID;
    GLuint NormalTextureID;
    GLuint SpecularTextureID;

    GLuint DiffuseTexture;
    GLuint NormalTexture;
    GLuint SpecularTexture;

    GLuint MatrixID;
    GLuint ViewMatrixID;
    GLuint ModelMatrixID;
    GLuint ModelView3x3MatrixID;

    std::vector<unsigned short> indices;
    glm::mat4 ModelMatrix;

};
