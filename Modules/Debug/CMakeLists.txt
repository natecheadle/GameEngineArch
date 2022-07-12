cmake_minimum_required(VERSION 3.21)
project("Modules_Debug")

set(CMAKE_CXX_STANDARD 20)

add_subdirectory("Logging")

add_library(${PROJECT_NAME} INTERFACE)

target_link_libraries(${PROJECT_NAME} INTERFACE Modules_Debug_Logging)