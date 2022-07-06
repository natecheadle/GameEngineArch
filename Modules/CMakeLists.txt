cmake_minimum_required(VERSION 3.21)
project("Modules")
add_subdirectory("Memory")
add_subdirectory("Jobs")
add_subdirectory("GUI")

add_library(${PROJECT_NAME} INTERFACE)

target_link_libraries(${PROJECT_NAME} INTERFACE Modules_Memory Modules_Jobs)