cmake_minimum_required(VERSION 3.21)

project(IgnosiGameEngine)

execute_process(
  COMMAND
    ${CMAKE_COMMAND} -E create_symlink
    ${CMAKE_BINARY_DIR}/compile_commands.json
    ${CMAKE_SOURCE_DIR}/compile_commands.json)

enable_testing()

add_compile_definitions(PUBLIC $<$<CONFIG:Debug>:DEBUG> "_USE_MATH_DEFINES")

add_subdirectory("Libraries")
add_subdirectory("Test")
add_subdirectory("Examples")
