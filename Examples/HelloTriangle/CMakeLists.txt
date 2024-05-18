cmake_minimum_required(VERSION 3.21)

project(Ignosi.Examples.HelloTriangle)

set(CMAKE_CXX_STANDARD 23)

add_executable(${PROJECT_NAME} "main.cpp" "HelloTriangleApp.cpp")

target_link_libraries(${PROJECT_NAME} PRIVATE Ignosi.Libraries.Renderer)

add_compile_definitions(OUT_DIR="${CMAKE_CURRENT_BINARY_DIR}")

add_custom_target(
  copy_triangle_assets
  ${CMAKE_COMMAND} -E copy_directory_if_different
  ${CMAKE_CURRENT_LIST_DIR}/assets ${CMAKE_CURRENT_BINARY_DIR}/assets
  COMMENT "Copying assets to output directory"
  VERBATIM)

add_dependencies(${PROJECT_NAME} copy_triangle_assets)
