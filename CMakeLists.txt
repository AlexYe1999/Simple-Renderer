cmake_minimum_required(VERSION 3.5)

project(main)

add_subdirectory(renderer)
add_subdirectory(model)
add_subdirectory(shader)
add_subdirectory(geometry)
add_subdirectory(texture)


set(STATIC_LIB 
    Renderer
    Model
    Shader
    Geometry
    Texture
)

add_executable(Rendering main.cpp)
target_link_libraries(Rendering ${STATIC_LIB})