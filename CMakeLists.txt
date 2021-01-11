cmake_minimum_required(VERSION 3.5)

project(main)

add_subdirectory(geometry)
add_subdirectory(renderer)
add_subdirectory(model)
add_subdirectory(shader)
add_subdirectory(texture)
add_subdirectory(object)
add_subdirectory(hitable_list)

set(STATIC_LIB 
    Renderer
    Model
    Shader
    Geometry
    Texture
    Object
    Hitable_List
)

#add_executable(Rendering main.cpp)
#target_link_libraries(Rendering ${STATIC_LIB})
add_executable(RayTracer RayTracer.cpp)
target_link_libraries(RayTracer ${STATIC_LIB})