cmake_minimum_required(VERSION 3.21)

set(CMAKE_CXX_STANDARD 20)

project(Ignosi.Libraries.Renderer)

find_package(glfw3 CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)

add_library(${PROJECT_NAME} STATIC IWindow.cpp GLFW_Window.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE glfw fmt::fmt)
