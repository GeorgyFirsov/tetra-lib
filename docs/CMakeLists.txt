#
# Configuration flags
#
if (TETRALIB_BUILD_PRETTY_DOCS)
    set(TETRALIB_DOXYGEN_HAVE_DOT              YES)
else (TETRALIB_BUILD_PRETTY_DOCS)
    set(TETRALIB_DOXYGEN_HAVE_DOT              NO)
endif (TETRALIB_BUILD_PRETTY_DOCS)

#
# Setup paths
#
set(TETRALIB_DOCS_VERSION                      ${CMAKE_PROJECT_VERSION})
set(TETRALIB_DOCS_ROOT                         ${TETRALIB_ROOT}/docs)
set(TETRALIB_DOCS_DOXYGEN_AWESOME_ROOT         ${TETRALIB_DOCS_ROOT}/doxygen-awesome-css)
set(TETRALIB_DOCS_DOXYGEN_IN                   ${TETRALIB_DOCS_ROOT}/Doxyfile.in)
set(TETRALIB_DOCS_DOXYGEN_OUT                  ${TETRALIB_DOCS_ROOT}/Doxyfile)

#
# And here we go...
#
message("[${PROJECT_NAME}]: Configuring doxygen...")
configure_file(${TETRALIB_DOCS_DOXYGEN_IN}     ${TETRALIB_DOCS_DOXYGEN_OUT} @ONLY)

#
# In case of GitHub docs we only need a configuration file to be 
# generated, so we quit here in this case
#
if (TETRALIB_BUILD_GITHUB_DOCS)
    message(NOTICE "[${PROJECT_NAME}]: Doxyfile is generated, but TETRALIB_BUILD_GITHUB_DOCS is set, nothing else will be performed")
    message(NOTICE "[${PROJECT_NAME}]: Doxyfile is located here: '${TETRALIB_DOCS_DOXYGEN_OUT}'")
    return()
endif (TETRALIB_BUILD_GITHUB_DOCS)

#
# Otherwise we need to find Doxygen
#
find_package(Doxygen)

#
# Check if we have Doxygen installed
#
if (NOT DOXYGEN_FOUND )
    message(WARNING "[${PROJECT_NAME}]: Doxygen need to be installed to generate the doxygen documentation")
    return()
endif() 

#
# Custom target for building documentation
#
add_custom_target(tetra-lib-docs ALL
    COMMAND ${DOXYGEN_EXECUTABLE} ${TETRALIB_DOCS_DOXYGEN_OUT}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating documentation with Doxygen"
    VERBATIM)
    