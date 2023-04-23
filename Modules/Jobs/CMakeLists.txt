cmake_minimum_required(VERSION 3.21)
project("Modules_Jobs")

set(CMAKE_CXX_STANDARD 20)

add_library(${PROJECT_NAME} STATIC "Job.cpp")
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})