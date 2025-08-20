cmake_minimum_required(VERSION 3.23)

add_executable(enum_ops_example enum_ops_example.cpp)
target_link_libraries(enum_ops_example PRIVATE stlab::enum-ops)
