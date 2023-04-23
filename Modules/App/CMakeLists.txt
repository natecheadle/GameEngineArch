cmake_minimum_required(VERSION 3.21)
project("Modules_App")

set(CMAKE_CXX_STANDARD 20)

add_library(${PROJECT_NAME} STATIC "App.cpp")
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME} PUBLIC "Modules_Render" "Modules_GUI" Modules_Jobs)