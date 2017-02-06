#include "Voxel.h"
#include "common/objloader.hpp"
#include "common/vboindexer.hpp"
#include "common/tangentspace.hpp"
#include "common/texture.hpp"

Voxel::Voxel()
{
    ModelMatrix = glm::mat4(1.0);
}

void Voxel::Load(GLuint programID)
{
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;
    bool res = loadOBJ("minecube.obj", vertices, uvs, normals);

    vertices.clear();
    uvs.clear();
    normals.clear();


    int yplane = 0.5;
    vertices.push_back(glm::vec3(-0.5,yplane,-0.5));
    vertices.push_back(glm::vec3(-0.5,yplane,0.5));
    vertices.push_back(glm::vec3(0.5,yplane,-0.5));

    vertices.push_back(glm::vec3(0.5,yplane,-0.5));
    vertices.push_back(glm::vec3(-0.5,yplane,0.5));
    vertices.push_back(glm::vec3(0.5,yplane,0.5));



    normals.push_back(glm::vec3(0,1,0));
    normals.push_back(glm::vec3(0,1,0));
    normals.push_back(glm::vec3(0,1,0));
    normals.push_back(glm::vec3(0,1,0));
    normals.push_back(glm::vec3(0,1,0));
    normals.push_back(glm::vec3(0,1,0));

    float blocksize = 16.0f/256.0f;


    uvs.push_back(glm::vec2(0,-1));
    uvs.push_back(glm::vec2(0,-(1-blocksize)));
    uvs.push_back(glm::vec2(blocksize,-1));

    uvs.push_back(glm::vec2(blocksize,-1));
    uvs.push_back(glm::vec2(0,-(1-blocksize)));
    uvs.push_back(glm::vec2(blocksize,-(1-blocksize)));


    std::vector<glm::vec3> indexed_vertices;
    std::vector<glm::vec2> indexed_uvs;
    std::vector<glm::vec3> indexed_normals;


    indexVBO( vertices, uvs, normals,
              indices, indexed_vertices, indexed_uvs, indexed_normals);

    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, indexed_vertices.size() * sizeof(glm::vec3), &indexed_vertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &uvbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    glBufferData(GL_ARRAY_BUFFER, indexed_uvs.size() * sizeof(glm::vec2), &indexed_uvs[0], GL_STATIC_DRAW);

    glGenBuffers(1, &normalbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
    glBufferData(GL_ARRAY_BUFFER, indexed_normals.size() * sizeof(glm::vec3), &indexed_normals[0], GL_STATIC_DRAW);


    // Generate a buffer for the indices as well
    glGenBuffers(1, &elementbuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0] , GL_STATIC_DRAW);


    // Load the texture
    //    DiffuseTexture = loadDDS("diffuse.DDS");
    DiffuseTexture = loadPNG("tex.png");
    NormalTexture = loadBMP_custom("normal.bmp");
    SpecularTexture = loadDDS("specular.DDS");

    // Get a handle for our "myTextureSampler" uniform
    DiffuseTextureID  = glGetUniformLocation(programID, "DiffuseTextureSampler");
    NormalTextureID  = glGetUniformLocation(programID, "NormalTextureSampler");
    SpecularTextureID  = glGetUniformLocation(programID, "SpecularTextureSampler");


    // Get a handle for our "MVP" uniform
    MatrixID = glGetUniformLocation(programID, "MVP");
    ViewMatrixID = glGetUniformLocation(programID, "V");
    ModelMatrixID = glGetUniformLocation(programID, "M");
    ModelView3x3MatrixID = glGetUniformLocation(programID, "MV3x3");
}

void Voxel::Unload()
{
    glDeleteBuffers(1, &vertexbuffer);
    glDeleteBuffers(1, &uvbuffer);
    glDeleteBuffers(1, &normalbuffer);
    glDeleteBuffers(1, &elementbuffer);
}

void Voxel::Draw(glm::mat4 ProjectionMatrix, glm::mat4 ViewMatrix)
{


    glm::mat4 ModelViewMatrix = ViewMatrix * ModelMatrix;
    glm::mat3 ModelView3x3Matrix = glm::mat3(ModelViewMatrix);
    glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

    // Send our transformation to the currently bound shader,
    // in the "MVP" uniform
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
    glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
    glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);
    glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);
    glUniformMatrix3fv(ModelView3x3MatrixID, 1, GL_FALSE, &ModelView3x3Matrix[0][0]);


    // Bind our texture in Texture Unit 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, DiffuseTexture);
    // Set our "DiffuseTextureSampler" sampler to user Texture Unit 0
    glUniform1i(DiffuseTextureID, 0);

    // Bind our normal texture in Texture Unit 1
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, NormalTexture);
    // Set our "Normal	TextureSampler" sampler to user Texture Unit 0
    glUniform1i(NormalTextureID, 1);

    // Bind our normal texture in Texture Unit 2
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, SpecularTexture);
    // Set our "Normal	TextureSampler" sampler to user Texture Unit 0
    glUniform1i(SpecularTextureID, 2);

    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(
                0,                  // attribute
                3,                  // size
                GL_FLOAT,           // type
                GL_FALSE,           // normalized?
                0,                  // stride
                (void*)0            // array buffer offset
                );

    // 2nd attribute buffer : UVs
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    glVertexAttribPointer(
                1,                                // attribute
                2,                                // size
                GL_FLOAT,                         // type
                GL_FALSE,                         // normalized?
                0,                                // stride
                (void*)0                          // array buffer offset
                );

    // 3rd attribute buffer : normals
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
    glVertexAttribPointer(
                2,                                // attribute
                3,                                // size
                GL_FLOAT,                         // type
                GL_FALSE,                         // normalized?
                0,                                // stride
                (void*)0                          // array buffer offset
                );

    // 4th attribute buffer : tangents
    glEnableVertexAttribArray(3);
    glBindBuffer(GL_ARRAY_BUFFER, tangentbuffer);
    glVertexAttribPointer(
                3,                                // attribute
                3,                                // size
                GL_FLOAT,                         // type
                GL_FALSE,                         // normalized?
                0,                                // stride
                (void*)0                          // array buffer offset
                );

    // 5th attribute buffer : bitangents
    glEnableVertexAttribArray(4);
    glBindBuffer(GL_ARRAY_BUFFER, bitangentbuffer);
    glVertexAttribPointer(
                4,                                // attribute
                3,                                // size
                GL_FLOAT,                         // type
                GL_FALSE,                         // normalized?
                0,                                // stride
                (void*)0                          // array buffer offset
                );

    // Index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);

    // Draw the triangles !
    glDrawElements(
                GL_TRIANGLES,      // mode
                indices.size(),    // count
                GL_UNSIGNED_SHORT, // type
                (void*)0           // element array buffer offset
                );

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(3);
    glDisableVertexAttribArray(4);
}


void Voxel::SetTranslation(GLfloat x, GLfloat y, GLfloat z)
{
    ModelMatrix[3][0] = -x;
    ModelMatrix[3][1] = -y;
    ModelMatrix[3][2] = -z;
}
