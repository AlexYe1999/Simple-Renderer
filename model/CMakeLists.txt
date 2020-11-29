aux_source_directory(. DIR_LIB_SRCS)

set(OpenCV_DIR /home/OpenCV/opencv-3.4.8/build)
find_package(OpenCV REQUIRED)
include_directories(
    ${OpenCV_INCLUDE_DIRS}
)

add_library (Model ${DIR_LIB_SRCS})
target_link_libraries(Model ${OpenCV_LIBS})