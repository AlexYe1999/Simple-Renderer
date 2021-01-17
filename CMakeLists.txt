cmake_minimum_required(VERSION 3.5)

project(RayTracer)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pg")
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg")
#set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -pg")

add_subdirectory(geometry)
add_subdirectory(renderer)
add_subdirectory(model)
add_subdirectory(shader)
add_subdirectory(texture)
add_subdirectory(object)
add_subdirectory(hitable_list)
add_subdirectory(material)

set(STATIC_LIB 
    Renderer
    Model
    Shader
    Geometry
    Texture
    Object
    Hitable_List
    Material
)

add_executable(Rendering main.cpp)
target_link_libraries(Rendering ${STATIC_LIB})
#add_executable(RayTracer RayTracer.cpp)
#target_link_libraries(RayTracer ${STATIC_LIB})
#add_executable(Games Games102/Games102.cpp)
#target_link_libraries(Games ${STATIC_LIB})