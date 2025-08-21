cmake_minimum_required(VERSION 3.20)
project(stlab-enum-ops VERSION 1.0.0 DESCRIPTION "Type-safe operators for enums" LANGUAGES CXX)

# Only setup full infrastructure when building as top-level project
if(PROJECT_IS_TOP_LEVEL)
    set(CPM_SOURCE_CACHE ${CMAKE_SOURCE_DIR}/.cpm-cache CACHE PATH "Directory to cache CPM packages" FORCE)
    include(cmake/CPM.cmake)
    
    # Fetch cpp-library via CPM (using local path for development)
    CPMAddPackage(
        NAME cpp-library
        SOURCE_DIR /Users/sean-parent/repos/github.com/stlab/cpp-library
        DOWNLOAD_ONLY YES
    )
    include(${cpp-library_SOURCE_DIR}/cpp-library.cmake)
    
    cpp_library_setup(
        NAME stlab-enum-ops
        VERSION ${PROJECT_VERSION}
        DESCRIPTION "${PROJECT_DESCRIPTION}"
        NAMESPACE stlab
        HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/include/stlab/enum_ops.hpp
        EXAMPLES enum_ops_example enum_ops_example_fail
        TESTS enum_ops_all_tests
        DOCS_EXCLUDE_SYMBOLS "stlab::implementation"
        ENABLE_CI YES                    # ← Enable CI generation
        CI_DEPLOY_DOCS YES              # ← Enable docs deployment
    )
else()
    # Lightweight consumer mode - just create the library target
    add_library(stlab-enum-ops INTERFACE)
    add_library(stlab::enum-ops ALIAS stlab-enum-ops)
    target_include_directories(stlab-enum-ops INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>)
    target_compile_features(stlab-enum-ops INTERFACE cxx_std_17)
endif()
