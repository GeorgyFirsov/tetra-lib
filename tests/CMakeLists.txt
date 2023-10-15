#
# Find googletest library
#
find_package(GTest CONFIG REQUIRED)

#
# Directories
#
set(TETRALIB_TESTS_ROOT				                ${TETRALIB_ROOT}/tests)
set(TETRALIB_TESTS_INCLUDE                          ${TETRALIB_TESTS_ROOT}/include)
set(TETRALIB_TESTS_CASES                            ${TETRALIB_TESTS_ROOT}/cases)
set(TETRALIB_TESTS_COVERAGE                         ${TETRALIB_TESTS_ROOT}/coverage)

set(TETRALIB_TESTS_INCLUDE_DIRECTORIES	            ${TETRALIB_INCLUDE_DIRECTORIES}
                                                    ${TETRALIB_TESTS_INCLUDE}
                                                    ${gtest_SOURCE_DIR}/include 
                                                    ${gtest_SOURCE_DIR})

#
# Sources and headers
#
set(TETRALIB_SOURCE_FILES                           ${TETRALIB_TESTS_CASES}/tea1.cpp)

set(TETRALIB_HEADER_FILES                           ${TETRALIB_TESTS_INCLUDE}/test_common.hpp
                                                    ${TETRALIB_TESTS_INCLUDE}/test_utils.hpp)

set(TETRALIB_SOURCES                                ${TETRALIB_SOURCE_FILES}
                                                    ${TETRALIB_HEADER_FILES})

#
# Test executable
#
add_executable(tetra-lib-test                       ${TETRALIB_SOURCES})

#
# Include directories
#
target_include_directories(tetra-lib-test PRIVATE   ${TETRALIB_TESTS_INCLUDE_DIRECTORIES})

#
# Link with tetra-lib and Google tests
#
target_link_libraries(tetra-lib-test PRIVATE        tetra-lib)
target_link_libraries(tetra-lib-test PRIVATE        GTest::gtest)
target_link_libraries(tetra-lib-test PRIVATE        GTest::gtest_main)

#
# Add target as test
#
add_test(NAME tetra-lib-test 
         COMMAND tetra-lib-test)
