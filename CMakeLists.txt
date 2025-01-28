cmake_minimum_required(VERSION 2.8)
project( Communication )
set(CMAKE_CXX_FLAGS "-std=c++23")

include_directories( include )

add_library(toolLib src/tools/dataStructs.c)

add_executable( main src/main.c )
target_link_libraries( main toolLib)