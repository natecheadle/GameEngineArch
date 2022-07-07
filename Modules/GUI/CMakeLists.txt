cmake_minimum_required(VERSION 3.21)
project("Modules_GUI")

set(CMAKE_CXX_STANDARD 20)

add_library(${PROJECT_NAME} STATIC
    "src/Window_GLFW.cpp")

find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc)