cmake_minimum_required(VERSION 2.8)
project( SendSocket )
set(CMAKE_CXX_FLAGS "-std=c++11")

include_directories( include
)

add_executable( sendSocket src/sendSocket.cpp)
# target_link_libraries( sen
# )