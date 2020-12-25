aux_source_directory(. DIR_LIB_SRCS)

find_package(OpenCV REQUIRED)
include_directories(
    ${OpenCV_INCLUDE_DIRS}
)

add_library (Texture ${DIR_LIB_SRCS})
target_link_libraries(Texture ${OpenCV_LIBS})