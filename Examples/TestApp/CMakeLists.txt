cmake_minimum_required(VERSION 3.21)

project(Ignosi.Examples.TestApp)

set(CMAKE_CXX_STANDARD 20)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE Ignosi.Libraries.Renderer)
