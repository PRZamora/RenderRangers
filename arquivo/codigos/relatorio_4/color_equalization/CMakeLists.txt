cmake_minimum_required(VERSION 2.8)
project( ColorEqualization )
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable( ColorEqualization ColorEqualization.cpp )
target_link_libraries( ColorEqualization ${OpenCV_LIBS} )
