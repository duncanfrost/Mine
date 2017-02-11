#pragma once
#include <GLFW/glfw3.h>

inline void APIENTRY DebugOutputCallback(GLenum source, GLenum type, GLuint id,
                                  GLenum severity,
                                         GLsizei length,
                                         const GLchar* message, const void* userParam)
{
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
