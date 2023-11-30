cmake_minimum_required(VERSION 3.21)

set(CMAKE_CXX_STANDARD 20)

project(Ignosi.Libraries.Math)

add_library(${PROJECT_NAME} STATIC "math_lib.cpp")
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
