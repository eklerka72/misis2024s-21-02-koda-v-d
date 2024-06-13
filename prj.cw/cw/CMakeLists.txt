file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS "./src/*.cpp")
add_executable(cw main.cpp ${PROJECT_SOURCES})
include_directories(headers)
target_link_libraries(cw PRIVATE opencv_core opencv_imgproc opencv_highgui)

