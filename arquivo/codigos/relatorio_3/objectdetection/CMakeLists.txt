cmake_minimum_required(VERSION 2.8)
project( ObjectIdentification )
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable( ObjectIdentification ObjectIdentification.cpp )
target_link_libraries( ObjectIdentification ${OpenCV_LIBS} )
