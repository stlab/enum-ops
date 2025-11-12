cmake_minimum_required(VERSION 3.20)

# Project declaration - cpp_library_setup will use this name and detect version from git tags
project(stlab-enum-ops)

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
CPMAddPackage("gh:stlab/cpp-library#69e1aff67499f3fb227f279a5472e46d312b3a84")
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
