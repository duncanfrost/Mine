# If you want to add/remove a test, modify here
set (TESTS
  )

# rule for binary build
foreach(test ${TESTS})
    # Compute the name of the binary to create
    get_filename_component(binary ${test} NAME_WE)

    # From test compile the binary and add link rules
    add_executable(${binary} ${test})
    target_link_libraries(${binary} ${INTERN_LIBS} ${EXTERN_LIBS})

    add_test (${binary} ${binary})
endforeach(test)
