cmake_minimum_required(VERSION 3.21)

set(CMAKE_CXX_STANDARD 20)

project(Ignosi.Libraries.ECS)

add_library(${PROJECT_NAME} STATIC "App.cpp")
target_link_libraries(${PROJECT_NAME} PUBLIC Ignosi.Libraries.Container
                                             Ignosi.Libraries.Generic)

target_include_directories(${PROJECT_NAME}
                           INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
