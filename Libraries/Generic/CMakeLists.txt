cmake_minimum_required(VERSION 3.21)

set(CMAKE_CXX_STANDARD 23)

project(Ignosi.Libraries.Generic)

add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME}
                           INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
