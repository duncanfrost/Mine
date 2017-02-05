configure_file(../shaders/SimpleFragmentShader.glsl SimpleFragmentShader.glsl COPYONLY)
configure_file(../shaders/SimpleVertexShader.glsl SimpleVertexShader.glsl COPYONLY)
configure_file(../shaders/TextFragmentShader.glsl TextFragmentShader.glsl COPYONLY)
configure_file(../shaders/TextVertexShader.glsl TextVertexShader.glsl COPYONLY)
configure_file(../data/uvtemplate.bmp uvtemplate.bmp COPYONLY)
configure_file(../data/uvtemplate.DDS uvtemplate.DDS COPYONLY)
configure_file(../data/cube.obj cube.obj COPYONLY)
configure_file(../data/suzanne.obj suzanne.obj COPYONLY)
configure_file(../data/uvmap.DDS uvmap.DDS COPYONLY)
configure_file(../data/uvmap2.DDS uvmap2.DDS COPYONLY)
configure_file(../data/Holstein.DDS Holstein.DDS COPYONLY)
configure_file(../data/cylinder.mtl cylinder.mtl COPYONLY)
configure_file(../data/cylinder.obj cylinder.obj COPYONLY)
configure_file(../data/diffuse.DDS diffuse.DDS COPYONLY)
configure_file(../data/normal.bmp normal.bmp COPYONLY)
configure_file(../data/specular.DDS specular.DDS COPYONLY)
configure_file(../data/tex.png tex.png COPYONLY)
configure_file(../data/tex2.png tex2.png COPYONLY)


# If you want to add/remove a source, modify here
SET (SOURCE
    main.cpp
    )

# rule for binary build
foreach(source ${SOURCE})
    # Compute the name of the binary to create
    GET_FILENAME_COMPONENT(binary ${source} NAME_WE)
    # From source compile the binary and add link rules
    ADD_EXECUTABLE(${binary} ${source})
    TARGET_LINK_LIBRARIES(${binary} ${INTERN_LIBS} ${EXTERN_LIBS})
endforeach(source)
