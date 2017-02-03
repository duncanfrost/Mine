#----------------------------------------------------------------------
# Include project files
#----------------------------------------------------------------------
# include all the Example project .cpp files
INCLUDE(${PROJECT_SOURCE_DIR}/CMakeSourceFileList.cmake)
# include all the Example project .h files
INCLUDE(${PROJECT_SOURCE_DIR}/CMakeHeaderFileList.cmake)


#----------------------------------------------------------------------
# Create a top level rule to copy all the headers from src to include/tracking
#----------------------------------------------------------------------
ADD_CUSTOM_TARGET(header ALL
  DEPENDS ${HEADER_IN_INCLUDE_DIR}
  )


#----------------------------------------------------------------------
# build rule for the library
#----------------------------------------------------------------------
ADD_LIBRARY (${INTERN_LIBS}
  ${SRC_ALL}
  ${HEADER_ALL}
  )

# create the headers in include/tracking before compiling the lib
ADD_DEPENDENCIES(${INTERN_LIBS} header)
