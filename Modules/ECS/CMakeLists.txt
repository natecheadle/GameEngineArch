cmake_minimum_required(VERSION 3.21)
project("Modules_ECS")

set(CMAKE_CXX_STANDARD 20)

add_library(${PROJECT_NAME} STATIC "Tag.cpp"  "CustomEntity.h")
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME} PUBLIC Modules_Generic Modules_Memory)
