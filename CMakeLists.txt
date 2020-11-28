cmake_minimum_required(VERSION 3.5)

project(main)

add_subdirectory(renderer)
add_executable(DrawLine main.cpp)

target_link_libraries(DrawLine Renderer)