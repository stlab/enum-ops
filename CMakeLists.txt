cmake_minimum_required(VERSION 3.20)

project(stlab-enum-ops VERSION 1.0.0 DESCRIPTION "Type-safe operators for enums" LANGUAGES CXX)

add_library(stlab-enum-ops INTERFACE)
add_library(stlab::enum-ops ALIAS stlab-enum-ops)

target_include_directories(stlab-enum-ops INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)

# Require at least C++17 for std::disjunction_v and other utilities
target_compile_features(stlab-enum-ops INTERFACE cxx_std_17)

include(CTest)

# Only download CPM when building tests/docs/examples as a top-level project
if(PROJECT_IS_TOP_LEVEL)
  set(CPM_SOURCE_CACHE ${CMAKE_SOURCE_DIR}/.cpm-cache CACHE PATH "Directory to cache CPM packages" FORCE)
  include(cmake/CPM.cmake)
endif()

if(BUILD_TESTING AND PROJECT_IS_TOP_LEVEL)
  # doctest dependency
  CPMAddPackage("gh:doctest/doctest@2.4.12")
  add_executable(enum_ops_tests tests/enum_ops_all_tests.cpp)
  target_link_libraries(enum_ops_tests PRIVATE stlab::enum-ops doctest::doctest)
  add_test(NAME enum_ops_tests COMMAND enum_ops_tests)

  # Add example programs (also used as tests to ensure they build and run)
  add_executable(enum_ops_example example/enum_ops_example.cpp)
  target_link_libraries(enum_ops_example PRIVATE stlab::enum-ops)
  add_test(NAME enum_ops_example COMMAND enum_ops_example)

  # Negative compile test: this example must fail to compile
  add_executable(enum_ops_example_fail EXCLUDE_FROM_ALL example/enum_ops_example_fail.cpp)
  target_link_libraries(enum_ops_example_fail PRIVATE stlab::enum-ops)
  add_test(
    NAME compile_enum_ops_example_fail
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target enum_ops_example_fail
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
  )
  set_tests_properties(compile_enum_ops_example_fail PROPERTIES WILL_FAIL TRUE)


  # Create symlink to compile_commands.json for clangd
  if(CMAKE_EXPORT_COMPILE_COMMANDS)
      add_custom_target(clangd_compile_commands ALL
          COMMAND ${CMAKE_COMMAND} -E create_symlink 
              ${CMAKE_BINARY_DIR}/compile_commands.json
              ${CMAKE_SOURCE_DIR}/compile_commands.json
          COMMENT "Creating symlink to compile_commands.json for clangd"
      )
  endif()
endif()

# Optional Doxygen docs target
option(BUILD_DOCS "Build documentation" OFF)
if(BUILD_DOCS AND PROJECT_IS_TOP_LEVEL)
  find_package(Doxygen REQUIRED)
  if(DOXYGEN_FOUND)
    CPMAddPackage(
      URI gh:jothepro/doxygen-awesome-css@2.3.4
      DOWNLOAD_ONLY YES
    )
    set(AWESOME_CSS_DIR ${doxygen-awesome-css_SOURCE_DIR})
    set(DOXYFILE_IN ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile)
    set(DOXYFILE_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
    # Inject version via configure
    configure_file(${DOXYFILE_IN} ${DOXYFILE_OUT})
    add_custom_target(docs
      COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE_OUT}
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
      COMMENT "Generating API documentation with Doxygen" VERBATIM)
  endif()
endif()
