cmake_minimum_required(VERSION 2.8)
project( CamEqualization )
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable( CamEqualization CamEqualization.cpp )
target_link_libraries( CamEqualization ${OpenCV_LIBS} )
