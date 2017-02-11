CMAKE_MINIMUM_REQUIRED(VERSION 2.8.12)
enable_testing()

set( CMAKE_EXPORT_COMPILE_COMMANDS true)

if(COMMAND cmake_policy)
    cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

SET(PROJECT_NAME "Mine")
PROJECT(${PROJECT_NAME})
SET (CMAKE_VERBOSE_MAKEFILE true)

set(CMAKE_MODULE_PATH
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake"
)




# The location in which to install binaries.
SET(PROJECT_SOURCE_DIR ${${PROJECT_NAME}_SOURCE_DIR})
SET(BINARY_OUTPUT_PATH ${${PROJECT_NAME}_BINARY_DIR}/bin)


# the internal visp libs we want to createn
SET(INTERN_LIBS tracking)

# the third party libraries we depend on
SET(EXTERN_LIBS "")


include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

#----------------------------------------------------------------------
# Third party libraries
#----------------------------------------------------------------------
# use sse3 instruction set
#SET(CMAKE_CXX_FLAGS "-Wall -Werror")
SET(CMAKE_CXX_FLAGS_RELEASE "-O3 -fopenmp -msse3")
SET(CMAKE_CXX_FLAGS_DEBUG "-g -msse3 -DNDEBUG")



#----------------------------------------------------------------------
# Options
#----------------------------------------------------------------------
option(BUILD_TESTS "Include unit tests and sample programs." ON)
option(USE_OPENGL "Use OpenGL" ON)
find_package(GLFW)
message("This is a test")
message(${GLFW_INCLUDE_DIR})

#----------------------------------------------------------------------
# GL
#----------------------------------------------------------------------
if (USE_OPENGL)
    find_package(GLUT)
    find_package(OpenGL)
    find_package(GLFW)
    find_package(PNG)
    message(${GLFW_INCLUDE_DIR})
    set(EXTERN_LIBS ${EXTERN_LIBS} ${GLUT_LIBRARY} ${OPENGL_LIBRARY} ${GLM_LIBRARY} ${GLFW_LIBRARY})
    set(EXTERN_LIBS ${EXTERN_LIBS} ${PNG_LIBRARY})
    set(EXTERN_LIBS ${EXTERN_LIBS} -lGLEW -lpng -lz -lboost_regex)
    INCLUDE_DIRECTORIES(${GLFW_INCLUDE_DIR})
endif(USE_OPENGL)




#----------------------------------------------------------------------
# Add library path
#----------------------------------------------------------------------
LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/lib)

#----------------------------------------------------------------------
# Propagation in sub dirs
#----------------------------------------------------------------------

SUBDIRS(src)
SUBDIRS(bin)
if (BUILD_TESTS)
    SUBDIRS(test)
endif(BUILD_TESTS)


