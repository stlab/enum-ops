cmake_minimum_required(VERSION 3.20)

# Project declaration - cpp_library_setup will use this name and detect version from git tags
project(enum-ops)

# Setup cpp-library infrastructure
if(PROJECT_IS_TOP_LEVEL)
    set(CPM_SOURCE_CACHE ${CMAKE_SOURCE_DIR}/.cache/cpm CACHE PATH "CPM cache")
endif()
include(cmake/CPM.cmake)

# Fetch cpp-library via CPM

# https://github.com/stlab/cpp-library/releases
# CPMAddPackage(
#     NAME cpp-library
#     URL "${CMAKE_SOURCE_DIR}/../cpp-library"
# )
CPMAddPackage("gh:stlab/cpp-library#4f09b3b7f0abcc4ef1b39060ce03784920ce32e5")
# CPMAddPackage("gh:stlab/cpp-library@4.0.5")

include(${cpp-library_SOURCE_DIR}/cpp-library.cmake)

# Let cpp-library handle the project declaration and version detection
cpp_library_setup(
    DESCRIPTION "Type-safe operators for enums"
    NAMESPACE stlab
    HEADERS enum_ops.hpp
    EXAMPLES enum_ops_example_test.cpp enum_ops_example_fail.cpp
    TESTS enum_ops_tests.cpp
    DOCS_EXCLUDE_SYMBOLS "stlab::implementation"
)
