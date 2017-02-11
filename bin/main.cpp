// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../src/common/shader.hpp"
#include "../src/common/controls.hpp"
#include "../src/common/text2D.hpp"

#include "../src/Voxel.h"
#include "../src/VoxelRenderer.h"
#include "../src/World.h"
#include "../src/Geometry.h"
#include "../src/Debug.h"
#include <iostream>

#include <stdexcept>
#include <vector>
#include <set>




int main( void )
{
    // Initialise GLFW
    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        getchar();
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // ARB_debug_output is a bit special,
    // it requires creating the OpenGL context
    // with paticular flags.
    // GLFW exposes it this way; if you use SDL, SFML, freeGLUT
    // or other, check the documentation.
    // If you use custom code, read the spec :
    // http://www.opengl.org/registry/specs/ARB/debug_output.txt
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, 1);

    // Open a window and create its OpenGL context
    window = glfwCreateWindow( 1024, 768, "Mine", NULL, NULL);
    if( window == NULL ){
        fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, "
                         "they are not 3.3 compatible. "
                         "Try the 2.1 version of the tutorials.\n" );
        getchar();
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return -1;
    }

    // Example 1 :
    if ( GLEW_AMD_seamless_cubemap_per_texture ){
        printf("The GL_AMD_seamless_cubemap_per_texture is present, (but we're not goint to use it)\n");
        // Now it's legal to call glTexParameterf with the TEXTURE_CUBE_MAP_SEAMLESS_ARB parameter
        // You HAVE to test this, because obviously, this code would fail on non-AMD hardware.
    }

    // Example 2 :
    if ( GLEW_ARB_debug_output ){
        printf("The OpenGL implementation provides debug output. Let's use it !\n");
        glDebugMessageCallbackARB(&DebugOutputCallback, NULL);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);
    }else{
        printf("ARB_debug_output unavailable. You have to use glGetError() "
               "and/or gDebugger to catch mistakes.\n");
    }


    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    // Hide the mouse and enable unlimited mouvement
#ifndef NDEBUG
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
#endif

    // Set the mouse at the center of the screen
    glfwPollEvents();
    glfwSetCursorPos(window, 1024/2, 768/2);

    // Dark blue background
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    // Cull triangles which normal is not towards the camera
    glEnable(GL_CULL_FACE); // Not this time !


    // Create and compile our GLSL program from the shaders
    GLuint programID = LoadShaders( "SimpleVertexShader.glsl", "SimpleFragmentShader.glsl" );






    // Get a handle for our "LightPosition" uniform
    glUseProgram(programID);
    GLuint LightID = glGetUniformLocation(programID, "LightPosition_worldspace");

    // For speed computation
    double lastTime = glfwGetTime();
    int nbFrames = 0;

    int ids[] = {2,7,1,3,4,13,16,56,15,73,21,11,14,9,12,18,17,82,49,48};
    std::map<int, Voxel> voxelMap;
    std::set<int> usedIDs;
    Voxel v;
    std::vector<int> idVector;
    for (unsigned int i = 0; i < 20; i++)
    {
        int id = ids[i];
        v.Load(programID,id);
        voxelMap.insert(std::make_pair(id,v));
        usedIDs.insert(id);
        idVector.push_back(id);
    }
    // Read our .obj file

    std::set<int> ignored;
    ignored.insert(31);
    ignored.insert(37);
    ignored.insert(38);
    ignored.insert(39);
    ignored.insert(10);
    ignored.insert(8);
    ignored.insert(83);
    ignored.insert(40);

    VoxelRenderer vr;
    vr.Load(programID,idVector);



    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    // open a region file
    World w;
    w.ReadFromFile("/home/duncan/.minecraft/saves/world/region/r.-1.0.mca", usedIDs);



    do{

        // Measure speed
        double currentTime = glfwGetTime();
        nbFrames++;
        if ( currentTime - lastTime >= 1.0 ){ // If last prinf() was more than 1sec ago
            // printf and reset
            std::cout << 1000.0/double(nbFrames) << "ms per frame" << std::endl;
            nbFrames = 0;
            lastTime += 1.0;
        }

        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use our shader
        glUseProgram(programID);

        // Compute the MVP matrix from keyboard and mouse input
        w.UpdatePlayerFromInputs(window);
        int offset = getOffset();
        glm::vec3 lightPos = w.GetPlayerPosition();
        glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);

        w.Draw(offset,vr);
        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

    } // Check if the ESC key was pressed or the window was closed
    while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0 );

    v.Unload();
    glfwTerminate();

    return 0;
}
