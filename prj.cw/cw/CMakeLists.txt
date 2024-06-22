file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS "./src/*.cpp")
add_executable(cw main.cpp ${PROJECT_SOURCES})
include_directories(headers)
target_link_libraries(cw PRIVATE opencv_core opencv_imgproc opencv_highgui)

install(TARGETS cw DESTINATION ${CMAKE_INSTALL_PREFIX}/bin.dbg)
install(
  DIRECTORY ${CMAKE_SOURCE_DIR}/bin.dbg
  DESTINATION ${CMAKE_INSTALL_PREFIX}
  FILES_MATCHING REGEX "[^\\\\/.]\\.[dD][lL][lL]$"
)
install(DIRECTORY "./images"
        DESTINATION ${CMAKE_INSTALL_PREFIX}/prj.cw/cw)
