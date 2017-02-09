# Set SRC_subdir variable to all the files we want to parse during
# the build process.
# Don't forget to update SRC_ALL variable if you add/remove a SRC_subdir
# variable
#
# If you add/remove a directory, modify here
set(SRC_MISC
    Defines.cpp
    Voxel.cpp
    World.cpp
    Chunk.cpp
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
set (SRC_LIBANV
    libanvil/byte_stream.cpp
    libanvil/chunk_info.cpp
    libanvil/chunk_tag.cpp
    libanvil/compression.cpp
    libanvil/region.cpp
    libanvil/region_file.cpp
    libanvil/region_file_reader.cpp
    libanvil/region_file_writer.cpp
    libanvil/region_header.cpp
    libanvil/tag/byte_array_tag.cpp
    libanvil/tag/byte_tag.cpp
    libanvil/tag/compound_tag.cpp
    libanvil/tag/double_tag.cpp
    libanvil/tag/end_tag.cpp
    libanvil/tag/float_tag.cpp
    libanvil/tag/generic_tag.cpp
    libanvil/tag/int_array_tag.cpp
    libanvil/tag/int_tag.cpp
    libanvil/tag/list_tag.cpp
    libanvil/tag/long_tag.cpp
    libanvil/tag/short_tag.cpp
    libanvil/tag/string_tag.cpp
    )

SET (SRC_ALL
    ${SRC_MISC}
    ${SRC_COMMON}
    ${SRC_LIBANV}
    )


