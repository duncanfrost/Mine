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
  Chunk.h
  VoxelRenderer.h
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
  common/ImageRead.h
  )

set (HEADER_LIBANV
  libanvil/byte_stream.hpp
  libanvil/chunk_info.hpp
  libanvil/chunk_tag.hpp
  libanvil/compression.hpp
  libanvil/region_dim.hpp
  libanvil/region_file.hpp
  libanvil/region_file_reader.hpp
  libanvil/region_file_writer.hpp
  libanvil/region_header.hpp
  libanvil/region.hpp
  libanvil/tag/byte_array_tag.hpp
  libanvil/tag/byte_tag.hpp
  libanvil/tag/compound_tag.hpp
  libanvil/tag/double_tag.hpp
  libanvil/tag/end_tag.hpp
  libanvil/tag/float_tag.hpp
  libanvil/tag/generic_tag.hpp
  libanvil/tag/int_array_tag.hpp
  libanvil/tag/int_tag.hpp
  libanvil/tag/list_tag.hpp
  libanvil/tag/long_tag.hpp
  libanvil/tag/short_tag.hpp
  libanvil/tag/string_tag.hpp
  )

SET (HEADER_ALL
  ${HEADER_MISC}
  ${HEADER_COMMON}
  ${HEADER_LIBANV}
  )
