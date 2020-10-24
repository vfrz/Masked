cmake_minimum_required(VERSION 3.17)
project(MaskedLBP)

set(CMAKE_CXX_STANDARD 20)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(MaskedLBP main.cpp main.h)

target_link_libraries(MaskedLBP ${OpenCV_LIBS})