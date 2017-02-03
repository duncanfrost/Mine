# Set SRC_subdir variable to all the files we want to parse during
# the build process.
# Don't forget to update SRC_ALL variable if you add/remove a SRC_subdir
# variable
#
# If you add/remove a directory, modify here
set(SRC_MISC
  Defines.cpp
  )
set (SRC_COMMON
  common/controls.cpp
  common/objloader.cpp
  common/quaternion_utils.cpp
  common/shader.cpp
  common/tangentspace.cpp
  common/text2D.cpp
  common/vboindexer.cpp
  common/texture.cpp
  )

SET (SRC_ALL
  ${SRC_MISC}
  ${SRC_COMMON}
  )

