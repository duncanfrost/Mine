#define REGION_CHUNKS 32*32
#include "Chunk.h"
#include <vector>
#include <string>
#include <set>
#include "VoxelRenderer.h"
#include <GLFW/glfw3.h>
#include <iostream>

class World
{

public:
    World();

    std::vector<Chunk> chunks;

    void ReadFromFile(std::string path, std::set<int> validIDs);

    void Draw(float offset, VoxelRenderer vr);
    void UpdatePlayerFromInputs(GLFWwindow *window);
    void UpdatePlayerEngine();
    void UpdateDeltaTime();

    glm::vec3 GetPlayerPosition()
    {
        return position;
    }

private:

    double lastTime;
    float deltaTime;

    bool runEngine;
    bool runEnginePressed;
    glm::vec3 position;
    glm::vec3 velocity;
    float horizontalAngle;
    float verticalAngle;
    float initialFoV;
    float speed;
    float mouseSpeed;
    float lastX, lastY;
    glm::mat4 ViewMatrix;
    glm::mat4 ProjectionMatrix;
};
