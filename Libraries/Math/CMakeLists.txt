cmake_minimum_required(VERSION 3.21)

set(CMAKE_CXX_STANDARD 23)

project(Ignosi.Libraries.Math)

find_package(gcem CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)

add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME}
                           INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME} INTERFACE Ignosi.Libraries.Generic gcem
                                                fmt::fmt-header-only )
