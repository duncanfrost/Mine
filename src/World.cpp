#include "World.h"
#include "libanvil/region_file_reader.hpp"
#include <glm/gtc/matrix_transform.hpp>

World::World()
{
    position = glm::vec3( 0, 0, 5 );
    horizontalAngle = 3.14f;
    // Initial vertical angle : none
    verticalAngle = 0.0f;
    // Initial Field of View
    initialFoV = 45.0f;
    speed = 3.0f; // 3 units / second
    mouseSpeed = 0.005f;
    lastX = 1024/2;
    lastY = 768 /2;
    runEngine = false;
}


void World::ReadFromFile(std::string path,
                         std::set<int> validIDs)
{

    region_file_reader reader = region_file_reader(path);
    reader.read();
    for (int rx = 0; rx < 32; rx++)
        for (int rz = 0; rz < 32; rz++)
        {
            // this keeps an exception from being thrown
            // when a non-existant chunk is requested
            if(!reader.is_filled(rx, rz))
                continue;

            // if everything goes well, retrieve the block/height data
            std::vector<int> blocks = reader.get_blocks_at(rx, rz);
            std::vector<int> heights = reader.get_heightmap_at(rx, rz);

            Chunk c;
            c.x = rx;
            c.z = rz;

            for (unsigned int x = 0; x < region_dim::BLOCK_WIDTH; x++)
                for (unsigned int z = 0; z < region_dim::BLOCK_WIDTH; z++)
                    for (unsigned int y = 0; y < region_dim::BLOCK_HEIGHT; y++)
                    {
                        int worldIndex = (y * 16 + z) * 16 + x;
                        int minecraftIndex = (y * 16 + z) * 16 + x;

                        if (minecraftIndex < blocks.size())
                        {
                            int id = blocks[minecraftIndex];
                            if (id > 0 && validIDs.count(id))
                                c.blockData[worldIndex] = id;
                            else
                                c.blockData[worldIndex] = 0;
                        }
                    }
            c.ComputeDrawData();
            chunks.push_back(c);
        }
}

void World::Draw(float offset, VoxelRenderer vr)
{

    for (int i = 0; i < chunks.size(); i++)
    {
        Chunk c = chunks[i];
        if (c.x > 4 || c.z > 4)
            continue;
        for (unsigned int x = 0; x < region_dim::BLOCK_WIDTH; x++)
            for (unsigned int z = 0; z < region_dim::BLOCK_WIDTH; z++)
                for (unsigned int y = 0; y < region_dim::BLOCK_HEIGHT; y++)
                {
                    int worldIndex = (y * 16 + z) * 16 + x;
                    if (c.blockData[worldIndex]  > 0 && c.drawData[worldIndex])
                    {
                        int id = c.blockData[worldIndex];
                        glm::vec3 translation(16*c.x + x,offset-y,16*c.z + z);
                        vr.Draw(ProjectionMatrix, ViewMatrix,translation,id);
                    }
                }
    }
}

void World::UpdateDeltaTime()
{
    double currentTime = glfwGetTime();
    deltaTime = float(currentTime - lastTime);
    lastTime = currentTime;
}


void World::UpdatePlayerFromInputs(GLFWwindow* window){


    // Compute time difference between current and last frame

    std::cout << deltaTime << std::endl;

    // Get mouse position
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);


    // Reset mouse position for next frame


    // Compute new orientation
    horizontalAngle += mouseSpeed * float(lastX - xpos );
    verticalAngle   = verticalAngle + mouseSpeed * float( lastY - ypos );

#ifdef NDEBUG
    lastX = xpos;
    lastY = ypos;
#else
    glfwSetCursorPos(window, 1024/2, 768/2);
#endif



    if (verticalAngle >= 1.5)
        verticalAngle = 1.5;

    if (verticalAngle <= -1.5)
        verticalAngle = -1.5;



    // Direction : Spherical coordinates to Cartesian coordinates conversion
    glm::vec3 direction(
        cos(verticalAngle) * sin(horizontalAngle),
        sin(verticalAngle),
        cos(verticalAngle) * cos(horizontalAngle)
    );

    // Right vector
    glm::vec3 right = glm::vec3(
        sin(horizontalAngle - 3.14f/2.0f),
        0,
        cos(horizontalAngle - 3.14f/2.0f)
    );

    // Up vector
    glm::vec3 up = glm::cross( right, direction );

    // Move forward
    if (glfwGetKey( window, GLFW_KEY_W ) == GLFW_PRESS){
        position += direction * deltaTime * speed;
    }
    // Move backward
    if (glfwGetKey( window, GLFW_KEY_S ) == GLFW_PRESS){
        position -= direction * deltaTime * speed;
    }
    // Strafe right
    if (glfwGetKey( window, GLFW_KEY_D ) == GLFW_PRESS){
        position += right * deltaTime * speed;
    }
    // Strafe left
    if (glfwGetKey( window, GLFW_KEY_A ) == GLFW_PRESS){
        position -= right * deltaTime * speed;
    }

    // Strafe left
    if (glfwGetKey( window, GLFW_KEY_LEFT_SHIFT ) == GLFW_PRESS){
        speed = 8;
    }
    else
        speed = 3;



    // Strafe left
    if (glfwGetKey( window, GLFW_KEY_I ) == GLFW_PRESS)
        runEnginePressed = true;

    // Strafe left
    if (glfwGetKey( window, GLFW_KEY_I ) == GLFW_RELEASE && runEnginePressed){
        runEnginePressed = false;
        runEngine = !runEngine;
        std::cout << "RunEngine: " << runEngine << std::endl;
    }


    float FoV = initialFoV;
    // - 5 * glfwGetMouseWheel(); // Now GLFW 3 requires
    //setting up a callback for this. It's a bit too complicated
    //for this beginner's tutorial, so it's disabled instead.

    // Projection matrix : 45 Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    ProjectionMatrix = glm::perspective(FoV, 4.0f / 3.0f, 0.1f, 100.0f);
    // Camera matrix
    ViewMatrix       = glm::lookAt(
                                position,           // Camera is here
                                position+direction, // and looks here : at the same position, plus "direction"
                                up                  // Head is up (set to 0,-1,0 to look upside-down)
                           );
}

void World::UpdatePlayerEngine()
{
    if (runEngine)
    {
        velocity[1] -= 10*deltaTime;
        position += velocity*deltaTime;
    }

}
