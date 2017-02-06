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
using namespace glm;

#include "../src/common/shader.hpp"
#include "../src/common/texture.hpp"
#include "../src/common/controls.hpp"
#include "../src/common/objloader.hpp"
#include "../src/common/vboindexer.hpp"
#include "../src/common/text2D.hpp"
#include "../src/common/tangentspace.hpp"

#include "../src/Voxel.h"
#include "../src/World.h"
#include <iostream>

#include <iostream>
#include <stdexcept>
#include <vector>
#include "../src/libanvil/region_file_reader.hpp"



void APIENTRY DebugOutputCallback(GLenum source, GLenum type, GLuint id,
                                  GLenum severity, GLsizei length, const GLchar* message, const void* userParam){

    printf("OpenGL Debug Output message : ");

    if(source == GL_DEBUG_SOURCE_API_ARB)					printf("Source : API; ");
    else if(source == GL_DEBUG_SOURCE_WINDOW_SYSTEM_ARB)	    printf("Source : WINDOW_SYSTEM; ");
    else if(source == GL_DEBUG_SOURCE_SHADER_COMPILER_ARB)	printf("Source : SHADER_COMPILER; ");
    else if(source == GL_DEBUG_SOURCE_THIRD_PARTY_ARB)		printf("Source : THIRD_PARTY; ");
    else if(source == GL_DEBUG_SOURCE_APPLICATION_ARB)		printf("Source : APPLICATION; ");
    else if(source == GL_DEBUG_SOURCE_OTHER_ARB)			    printf("Source : OTHER; ");

    if(type == GL_DEBUG_TYPE_ERROR_ARB)						printf("Type : ERROR; ");
    else if(type == GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB)	printf("Type : DEPRECATED_BEHAVIOR; ");
    else if(type == GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB)	    printf("Type : UNDEFINED_BEHAVIOR; ");
    else if(type == GL_DEBUG_TYPE_PORTABILITY_ARB)			printf("Type : PORTABILITY; ");
    else if(type == GL_DEBUG_TYPE_PERFORMANCE_ARB)			printf("Type : PERFORMANCE; ");
    else if(type == GL_DEBUG_TYPE_OTHER_ARB)				printf("Type : OTHER; ");

    if(severity == GL_DEBUG_SEVERITY_HIGH_ARB)				printf("Severity : HIGH; ");
    else if(severity == GL_DEBUG_SEVERITY_MEDIUM_ARB)		printf("Severity : MEDIUM; ");
    else if(severity == GL_DEBUG_SEVERITY_LOW_ARB)			printf("Severity : LOW; ");

    // You can set a breakpoint here ! Your debugger will stop the program,
    // and the callstack will immediately show you the offending call.
    printf("Message : %s\n", message);
}

int main( void )
{

    World w;
    Voxel v1;
//    v1.Load(1);
    // instantiate a file reader
    region_file_reader reader;

    // create vectors to hold block/height data
    std::vector<int> blocks, heights;



    // open a region file
    reader = region_file_reader("/home/duncan/.minecraft/saves/world/region/r.-1.0.mca");
    reader.read();

    blocks = reader.get_blocks_at(0, 0);

    // iterate through all possible chunks within a region


    for (int rx = 0; rx < 32; rx++)
        for (int rz = 0; rz < 32; rz++)
        {
            // this keeps an exception from being thrown
            // when a non-existant chunk is requested
            if(!reader.is_filled(rx, rz))
            {
                std::cout << "No blocks here" << std::endl;
                continue;
            }

            // if everything goes well, retrieve the block/height data
            blocks = reader.get_blocks_at(rx, rz);
            heights = reader.get_heightmap_at(rx, rz);

            std::cout << "Got blocks at " << rx << "," << rz << std::endl;
            int chunkIndex = rx + 32*rz;

            for (unsigned int x = 0; x < w.width; x++)
                for (unsigned int z = 0; z < w.breadth; z++)
                    for (unsigned int y = 0; y < w.depth; y++)
                    {
                        int worldIndex = (y * 16 + z) * 16 + x;
                        int minecraftIndex = (y * 16 + z) * 16 + x;

                        if (minecraftIndex < blocks.size())
                        {
                            if (blocks[minecraftIndex] > 0)
                                w.blockData[chunkIndex][worldIndex] = 1;
                            else
                                w.blockData[chunkIndex][worldIndex] = 0;
                }
            }
        }





    w.blockData[0][0] = 1;

    loadPNG("tex2.png");


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
    window = glfwCreateWindow( 1024, 768, "Tutorial 12 - Extensions", NULL, NULL);
    if( window == NULL ){
        fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
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
        printf("ARB_debug_output unavailable. You have to use glGetError() and/or gDebugger to catch mistakes.\n");
    }


    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    // Hide the mouse and enable unlimited mouvement
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Set the mouse at the center of the screen
    glfwPollEvents();
    glfwSetCursorPos(window, 1024/2, 768/2);

    // Dark blue background
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    // Cull triangles which normal is not towards the camera
    glEnable(GL_CULL_FACE); // Not this time !

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    // Create and compile our GLSL program from the shaders
    GLuint programID = LoadShaders( "SimpleVertexShader.glsl", "SimpleFragmentShader.glsl" );


    // Read our .obj file
    Voxel v;
    v.Load(programID);



    //    for (unsigned int x = 0; x < w.width; x++)
    //            for (unsigned int z = 0; z < w.depth; z++)
    //            {
    //                w.SetBlock(x,0,z);
    //                w.SetBlock(x,9,z);
    //            }



    // Get a handle for our "LightPosition" uniform
    glUseProgram(programID);
    GLuint LightID = glGetUniformLocation(programID, "LightPosition_worldspace");

    // For speed computation
    double lastTime = glfwGetTime();
    int nbFrames = 0;

    // Enable blending
    //    glEnable(GL_BLEND);
    //    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    do{

        // Measure speed
        double currentTime = glfwGetTime();
        nbFrames++;
        if ( currentTime - lastTime >= 1.0 ){ // If last prinf() was more than 1sec ago
            // printf and reset
            printf("%f ms/frame\n", 1000.0/double(nbFrames));
            nbFrames = 0;
            lastTime += 1.0;
        }

        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use our shader
        glUseProgram(programID);

        // Compute the MVP matrix from keyboard and mouse input
        computeMatricesFromInputs();
        int chunkNumber = getChunkNumber();
        int offset = getOffset();
        glm::mat4 ProjectionMatrix = getProjectionMatrix();
        glm::mat4 ViewMatrix = getViewMatrix();




        glm::vec3 lightPos = glm::vec3(4,4,4);
        glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);



        for (unsigned int x = 0; x < w.width; x++)
            for (unsigned int z = 0; z < w.breadth; z++)
                for (unsigned int y = 0; y < w.depth; y++)
                {
                    int worldIndex = (y * 16 + z) * 16 + x;
                    if (w.blockData[chunkNumber][worldIndex]  == 1)
                    {
                        v.SetTranslation(x,offset-y,z);
                        v.Draw(ProjectionMatrix, ViewMatrix);
                    }
                }
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
