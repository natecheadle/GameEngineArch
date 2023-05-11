cmake_minimum_required(VERSION 3.21)
project("Modules_Generic")

set(CMAKE_CXX_STANDARD 20)

find_package(fmt CONFIG REQUIRED)

add_library(${PROJECT_NAME} INTERFACE)

target_link_libraries(${PROJECT_NAME} INTERFACE fmt::fmt)
target_include_directories(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
