cmake_minimum_required(VERSION 3.5.0)
project(tetra-lib VERSION 1.0.0)

#
# Global config
#
set(CMAKE_CXX_STANDARD                              17)
set(CMAKE_CXX_STANDARD_REQUIRED                     True)

#
# Options
#
option(TETRALIB_GENERATE_DOCS                       "Generate documentation for tetra-lib." OFF)
option(TETRALIB_DOCS_ONLY                           "Generate documentation only (tetra-lib will not be built)." OFF)
option(TETRALIB_GITHUB_DOCS                         "Generate documentation for GitHub." OFF)
option(TETRALIB_PRETTY_DOCS                         "Use graphwiz for diagrams." OFF)
option(TETRALIB_ENABLE_TESTING                      "Enable testing of ciphers, modes of operation and other functions." ON)

#
# Configuration
#
if (WIN32)
    set(TETRALIB_WINDOWS_BUILD                      ON)
else (WIN32)
    set(TETRALIB_WINDOWS_BUILD                      OFF)
endif (WIN32)

if (TETRALIB_GENERATE_DOCS AND TETRALIB_DOCS_ONLY)

    #
    # Building docs only
    #

    set(TETRALIB_BUILD_LIB                          OFF)
    set(TETRALIB_BUILD_TESTS                        OFF)
    set(TETRALIB_BUILD_DOCS                         ON)
    set(TETRALIB_BUILD_GITHUB_DOCS                  ${TETRALIB_GITHUB_DOCS})
    set(TETRALIB_BUILD_PRETTY_DOCS                  ${TETRALIB_PRETTY_DOCS})

else (TETRALIB_GENERATE_DOCS AND TETRALIB_DOCS_ONLY)

    #
    # Here I assume other flags, but library 
    # will be built always here.
    #

    set(TETRALIB_BUILD_LIB                          ON)
    set(TETRALIB_BUILD_TESTS                        ${TETRALIB_ENABLE_TESTING})
    set(TETRALIB_BUILD_DOCS                         ${TETRALIB_GENERATE_DOCS})
    set(TETRALIB_BUILD_GITHUB_DOCS                  ${TETRALIB_GITHUB_DOCS})
    set(TETRALIB_BUILD_PRETTY_DOCS                  ${TETRALIB_PRETTY_DOCS})

endif (TETRALIB_GENERATE_DOCS AND TETRALIB_DOCS_ONLY)

message("[${PROJECT_NAME}]: TETRALIB_WINDOWS_BUILD     = ${TETRALIB_WINDOWS_BUILD}")
message("[${PROJECT_NAME}]: TETRALIB_BUILD_LIB         = ${TETRALIB_BUILD_LIB}")
message("[${PROJECT_NAME}]: TETRALIB_BUILD_TESTS       = ${TETRALIB_BUILD_TESTS}")
message("[${PROJECT_NAME}]: TETRALIB_BUILD_DOCS        = ${TETRALIB_BUILD_DOCS}")
message("[${PROJECT_NAME}]: TETRALIB_BUILD_GITHUB_DOCS = ${TETRALIB_BUILD_GITHUB_DOCS}")
message("[${PROJECT_NAME}]: TETRALIB_BUILD_PRETTY_DOCS = ${TETRALIB_BUILD_PRETTY_DOCS}")

#
# Just sanity check if we are going well
#
if (NOT TETRALIB_BUILD_LIB AND TETRALIB_BUILD_TESTS)
    message(FATAL_ERROR "[${PROJECT_NAME}]: cannot build tests without building tetra-lib itself")
endif (NOT TETRALIB_BUILD_LIB AND TETRALIB_BUILD_TESTS)

#
# Set global directories
#
set(TETRALIB_ROOT                                   ${CMAKE_CURRENT_LIST_DIR})
set(TETRALIB_SOURCES_ROOT                           ${TETRALIB_ROOT}/src)
set(TETRALIB_INCLUDE_ROOT                           ${TETRALIB_ROOT}/include)
set(TETRALIB_INCLUDE_DIRECTORIES                    ${TETRALIB_INCLUDE_ROOT})

#
# Set output directories for all targets
#
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY                  ${CMAKE_BINARY_DIR}/out)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY                  ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY                  ${CMAKE_BINARY_DIR}/lib)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE          ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE          ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE          ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG            ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG            ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG            ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})

#
# Now let's build targets according to the flags set before
#
if (TETRALIB_BUILD_LIB)

    #
    # Implementation directories
    #
    set(TETRALIB_DETAILS_SOURCES_DIR                ${TETRALIB_SOURCES_ROOT}/details)
    set(TETRALIB_DETAILS_INCLUDE_DIR                ${TETRALIB_INCLUDE_ROOT}/details)

    set(TETRALIB_INTERNAL_INCLUDE_DIRECTORIES       ${TETRALIB_INCLUDE_DIRECTORIES})

    #
    # Source files
    #
    set(TETRALIB_SOURCE_FILES                       ${TETRALIB_DETAILS_SOURCES_DIR}/tea1.c
                                                    ${TETRALIB_DETAILS_SOURCES_DIR}/tea2.c
                                                    ${TETRALIB_DETAILS_SOURCES_DIR}/tea3.c)

    set(TETRALIB_HEADER_FILES                       ${TETRALIB_DETAILS_INCLUDE_DIR}/tea1.h
                                                    ${TETRALIB_DETAILS_INCLUDE_DIR}/tea2.h
                                                    ${TETRALIB_DETAILS_INCLUDE_DIR}/tea3.h)

    set(TETRALIB_SOURCES                            ${TETRALIB_SOURCE_FILES}
                                                    ${TETRALIB_HEADER_FILES})

    #
    # Library itself (may be built for user mode as 
    # well as for kernel mode)
    #
    add_library(tetra-lib                           ${TETRALIB_SOURCES})

    #
    # Include directories
    #
    target_include_directories(tetra-lib PRIVATE    ${TETRALIB_INTERNAL_INCLUDE_DIRECTORIES})

    #
    # On non-Windows builds it is necessary to add pclmul support
    #
    if (NOT TETRALIB_WINDOWS_BUILD)
        target_compile_options(tetra-lib PRIVATE    -mpclmul)
    endif (NOT TETRALIB_WINDOWS_BUILD)

endif (TETRALIB_BUILD_LIB)

if (TETRALIB_BUILD_TESTS)

    include(CTest)

    #
    # Tests target
    #
    add_subdirectory(tests)

endif (TETRALIB_BUILD_TESTS)

if (TETRALIB_BUILD_DOCS)

    #
    # Docs target
    #
    add_subdirectory(docs)

endif (TETRALIB_BUILD_DOCS)
