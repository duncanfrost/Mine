# Set HEADER_subdir variable to all the files we want to parse during
# the build process.
# Don't forget to update HEADER_ALL variable if you add/remove a
# HEADER_subdir variable
#
# If you add/remove a directory, modify here

set(HEADER_MISC
  Defines.h
  Voxel.h
  World.h
  )

set (HEADER_COMMON
  common/controls.hpp
  common/objloader.hpp
  common/quaternion_utils.hpp
  common/shader.hpp
  common/tangentspace.hpp
  common/text2D.hpp
  common/texture.hpp
  common/vboindexer.hpp
  )

SET (HEADER_ALL
  ${HEADER_MISC}
  ${HEADER_COMMON}
  )


