cmake_minimum_required(VERSION 3.24)

include(cmake/CPM.cmake)

# Fetch cpp-library before project()
# Check https://github.com/stlab/cpp-library/releases for the latest version
# CPMAddPackage(
#     NAME cpp-library
#     SOURCE_DIR "${CMAKE_SOURCE_DIR}/../cpp-library"
# )
# CPMAddPackage("gh:stlab/cpp-library#65dbed9fff9a0331355bd51dc1e8156262390154")
CPMAddPackage("gh:stlab/cpp-library@5.0.0")
include(${cpp-library_SOURCE_DIR}/cpp-library.cmake)

# Enable dependency tracking before project()
cpp_library_enable_dependency_tracking()

# Now declare project
project(enum-ops)

# Enable CTest infrastructure (required for tests/examples to work)
include(CTest)

# Let cpp-library handle the project declaration and version detection
cpp_library_setup(
    DESCRIPTION "Type-safe operators for enums"
    NAMESPACE stlab
    HEADERS enum_ops.hpp
    EXAMPLES enum_ops_example_test.cpp enum_ops_example_fail.cpp
    TESTS enum_ops_tests.cpp
    DOCS_EXCLUDE_SYMBOLS "stlab::implementation"
)
