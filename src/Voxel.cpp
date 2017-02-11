#include "Voxel.h"
#include "common/objloader.hpp"
#include "common/vboindexer.hpp"
#include "common/tangentspace.hpp"
#include "common/texture.hpp"
#include <iostream>

Voxel::Voxel()
{
    ModelMatrix = glm::mat4(1.0);
}


void GenUVS(int gridX, int gridY, glm::vec3 normal, std::vector<glm::vec2> &uvs)
{
    float blocksize = 16.0f/256.0f;
    if (normal[0] == -1 || normal[2] == 1 || normal[1] == 1)
    {
        uvs.push_back(glm::vec2(gridX*blocksize,-1 +(gridY+1)*blocksize));
        uvs.push_back(glm::vec2((gridX+1)*blocksize,-1 +(gridY+1)*blocksize));
        uvs.push_back(glm::vec2(gridX*blocksize,-1 + gridY*blocksize));

        uvs.push_back(glm::vec2((gridX)*blocksize,-1 + (gridY)*blocksize));
        uvs.push_back(glm::vec2((gridX+1)*blocksize,-1 +(gridY+1)*blocksize));
        uvs.push_back(glm::vec2((gridX+1)*blocksize,-1 +(gridY)*blocksize));
    }
    else
    {
        uvs.push_back(glm::vec2((gridX+1)*blocksize,-1 +(gridY+1)*blocksize));
        uvs.push_back(glm::vec2((gridX+1)*blocksize,-1 +(gridY)*blocksize));
        uvs.push_back(glm::vec2(gridX*blocksize,-1 +(gridY+1)*blocksize));

        uvs.push_back(glm::vec2((gridX)*blocksize,-1 +(gridY+1)*blocksize));
        uvs.push_back(glm::vec2((gridX+1)*blocksize,-1 +(gridY)*blocksize));
        uvs.push_back(glm::vec2(gridX*blocksize,-1 +(gridY)*blocksize));
    }
}


void GenSide(glm::vec3 normal,
             std::vector<glm::vec3> &vertices,
             std::vector<glm::vec3> &normals
             )

{

    float yplane = 0.5;

    int otherindex1, otherindex2;
    int planeIndex;

    float normalSign;

    if (abs(normal[0]) == 1)
    {
        planeIndex = 0;
        otherindex1 = 2;
        otherindex2 = 1;
    }
    if (abs(normal[1]) == 1)
    {
        planeIndex = 1;
        otherindex1 = 0;
        otherindex2 = 2;
    }
    if (abs(normal[2]) == 1)
    {
        planeIndex = 2;
        otherindex1 = 1;
        otherindex2 = 0;
    }


    normalSign = normal[planeIndex];
    yplane *= normalSign;
    if (normalSign < 0)
    {
        int tmp = otherindex1;
        otherindex1 = otherindex2;
        otherindex2 = tmp;
    }

    glm::vec3 inputVec;
    inputVec[otherindex1] = -0.5;
    inputVec[otherindex2] = -0.5;
    inputVec[planeIndex] = yplane;

    vertices.push_back(inputVec);

    inputVec[otherindex2] = 0.5;
    vertices.push_back(inputVec);

    inputVec[otherindex2] = -0.5;
    inputVec[otherindex1] = 0.5;
    vertices.push_back(inputVec);



    vertices.push_back(inputVec);

    inputVec[otherindex1] = -0.5;
    inputVec[otherindex2] = 0.5;
    vertices.push_back(inputVec);

    inputVec[otherindex1] = 0.5;
    vertices.push_back(inputVec);

    normals.push_back(normal);
    normals.push_back(normal);
    normals.push_back(normal);
    normals.push_back(normal);
    normals.push_back(normal);
    normals.push_back(normal);
}

void Voxel::Load(GLuint programID, int ID)
{
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;

    vertices.clear();
    uvs.clear();
    normals.clear();

    int topUVX, topUVY;
    int sideUVX, sideUVY;
    int bottomUVX, bottomUVY;

    switch (ID) {
    case 7:
        topUVX = topUVY = sideUVX = sideUVY = bottomUVX = bottomUVY = 1;
        break;
    case 1:
        topUVX = bottomUVX = sideUVX = 1;
        topUVY = bottomUVY = sideUVY = 0;
        break;
    case 3:
        topUVX = bottomUVX = sideUVX = 2;
        topUVY = bottomUVY = sideUVY = 0;
        break;
    case 2:
        sideUVX = 3;
        topUVX = 0;
        bottomUVX = 2;
        sideUVY = topUVY = bottomUVY = 0;
        break;
    case 13:
        topUVX = bottomUVX = sideUVX = 3;
        topUVY = bottomUVY = sideUVY = 1;
        break;
    case 15:
        topUVX = bottomUVX = sideUVX = 1;
        topUVY = bottomUVY = sideUVY = 2;
        break;
    case 16:
        topUVX = bottomUVX = sideUVX = 2;
        topUVY = bottomUVY = sideUVY = 2;
        break;
    case 56:
        topUVX = bottomUVX = sideUVX = 2;
        topUVY = bottomUVY = sideUVY = 3;
        break;
    case 21:
        topUVX = bottomUVX = sideUVX = 0;
        topUVY = bottomUVY = sideUVY = 10;
        break;
    case 11:
        topUVX = bottomUVX = sideUVX = 14;
        topUVY = bottomUVY = sideUVY = 14;
        break;
    case 73:
        topUVX = bottomUVX = sideUVX = 3;
        topUVY = bottomUVY = sideUVY = 3;
        break;
    case 14:
        topUVX = bottomUVX = sideUVX = 0;
        topUVY = bottomUVY = sideUVY = 2;
        break;
    case 9:
        topUVX = bottomUVX = sideUVX = 0;
        topUVY = bottomUVY = sideUVY = 9;
        break;
    case 12:
        topUVX = bottomUVX = sideUVX = 1;
        topUVY = bottomUVY = sideUVY = 2;
        break;
    case 18:
        topUVX = bottomUVX = sideUVX = 5;
        topUVY = bottomUVY = sideUVY = 3;
        break;
    case 82:
        topUVX = bottomUVX = sideUVX = 8;
        topUVY = bottomUVY = sideUVY = 4;
        break;
    case 49:
        topUVX = bottomUVX = sideUVX = 5;
        topUVY = bottomUVY = sideUVY = 2;
        break;
    case 48:
        topUVX = bottomUVX = sideUVX = 4;
        topUVY = bottomUVY = sideUVY = 2;
        break;
    case 4:
        topUVX = bottomUVX = sideUVX = 0;
        topUVY = bottomUVY = sideUVY = 1;
        break;
    case 17:
        topUVY = bottomUVY = sideUVY = 1;
        topUVX = bottomUVX = 5;
        sideUVX = 4;
        break;
    default:
        std::cout << "I don't have uv coordinates for this block" << std::endl;
        exit(1);
    }

    GenSide(glm::vec3(0,1,0),vertices,normals);
    GenSide(glm::vec3(0,-1,0),vertices,normals);
    GenSide(glm::vec3(0,0,1),vertices,normals);
    GenSide(glm::vec3(1,0,0),vertices,normals);
    GenSide(glm::vec3(-1,0,0),vertices,normals);
    GenSide(glm::vec3(0,0,-1),vertices,normals);
    GenUVS(topUVX,topUVY,glm::vec3(0,1,0),uvs);
    GenUVS(bottomUVX,bottomUVY,glm::vec3(0,-1,0),uvs);
    GenUVS(sideUVX,sideUVY,glm::vec3(0,0,1),uvs);
    GenUVS(sideUVX,sideUVY,glm::vec3(1,0,0),uvs);
    GenUVS(sideUVX,sideUVY,glm::vec3(-1,0,0),uvs);
    GenUVS(sideUVX,sideUVY,glm::vec3(0,0,-1),uvs);




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

    // Get a handle for our "myTextureSampler" uniform
    DiffuseTextureID  = glGetUniformLocation(programID, "DiffuseTextureSampler");


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
}


void Voxel::SetTranslation(GLfloat x, GLfloat y, GLfloat z)
{
    ModelMatrix[3][0] = -x;
    ModelMatrix[3][1] = -y;
    ModelMatrix[3][2] = -z;
}
