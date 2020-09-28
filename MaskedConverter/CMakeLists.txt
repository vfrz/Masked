cmake_minimum_required(VERSION 3.17)
project(MaskedConverter)

set(CMAKE_CXX_STANDARD 20)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(MaskedConverter main.cpp)

target_link_libraries(MaskedConverter ${OpenCV_LIBS})