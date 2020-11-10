cmake_minimum_required(VERSION 3.17)
project(Masked)

set(CMAKE_CXX_STANDARD 20)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(Masked main.cpp Trainer.cpp Trainer.h Predictor.cpp Predictor.h MaskedType.h MaskedLBPModel.cpp MaskedLBPModel.h ComparisonAlgorithm.h)

target_link_libraries(Masked ${OpenCV_LIBS})