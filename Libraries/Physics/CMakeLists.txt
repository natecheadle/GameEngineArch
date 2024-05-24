cmake_minimum_required(VERSION 3.21)

set(CMAKE_CXX_STANDARD 20)

project(Ignosi.Libraries.Physics)

add_library(${PROJECT_NAME} STATIC "PhysicsEngine.cpp")

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME} PUBLIC Ignosi.Libraries.Math
                                             Ignosi.Libraries.ECS)
