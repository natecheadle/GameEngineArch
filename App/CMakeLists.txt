cmake_minimum_required(VERSION 3.21)
project(App)

set(CMAKE_CXX_STANDARD 20)

add_executable(${PROJECT_NAME} 
    "main.cpp")

target_link_libraries(${PROJECT_NAME} PRIVATE Modules)

BUILD_SHADERS("Shaders/fs_cubes.sc" "Shaders/vs_cubes.sc")