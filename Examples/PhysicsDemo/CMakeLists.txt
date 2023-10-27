cmake_minimum_required(VERSION 3.21)
project(PhysicsDemo)

set(CMAKE_CXX_STANDARD 20)

add_executable(${PROJECT_NAME} main.cpp PhysicsDemoApp.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE Modules)
target_compile_definitions(
  ${PROJECT_NAME} PRIVATE "APP_OUT_DIR=\"${CMAKE_CURRENT_BINARY_DIR}\"")
