cmake_minimum_required(VERSION 3.20)
project(stlab-enum-ops VERSION 1.0.0 DESCRIPTION "Type-safe operators for enums" LANGUAGES CXX)

# Setup cpp-library infrastructure
set(CPM_SOURCE_CACHE ${CMAKE_SOURCE_DIR}/.cpm-cache CACHE PATH "CPM cache" FORCE)
include(cmake/CPM.cmake)

# Fetch cpp-library via CPM (using local path for development)
CPMAddPackage(
    NAME cpp-library
    SOURCE_DIR /Users/sean-parent/repos/github.com/stlab/cpp-library
    DOWNLOAD_ONLY YES
)
include(${cpp-library_SOURCE_DIR}/cpp-library.cmake)


# Configure library (handles both lightweight and full modes automatically)
cpp_library_setup(
    NAME stlab-enum-ops
    VERSION ${PROJECT_VERSION}
    DESCRIPTION "${PROJECT_DESCRIPTION}"
    NAMESPACE stlab
    HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/include/stlab/enum_ops.hpp
    EXAMPLES enum_ops_example enum_ops_example_fail
    TESTS enum_ops_tests
    DOCS_EXCLUDE_SYMBOLS "stlab::implementation"
)
