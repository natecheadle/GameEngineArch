cmake_minimum_required(VERSION 3.21)
project(BreakOut)

set(CMAKE_CXX_STANDARD 20)

add_executable(${PROJECT_NAME} 
    "main.cpp"
    "BreakOutApp.cpp"
    "Ball.cpp"
    "Brick.cpp"
    "Level.cpp"
    "Paddle.cpp")


add_compile_definitions(SHADER_DIR=R"\(${CMAKE_CURRENT_SOURCE_DIR}\)")

target_link_libraries(${PROJECT_NAME} PRIVATE Modules)
target_compile_definitions(${PROJECT_NAME} PRIVATE "APP_OUT_DIR=\"${CMAKE_CURRENT_BINARY_DIR}\"")
