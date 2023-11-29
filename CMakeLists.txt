cmake_minimum_required(VERSION 3.21)

project(IgnosiGameEngine)

enable_testing()

add_compile_definitions(PUBLIC $<$<CONFIG:Debug>:DEBUG> "_USE_MATH_DEFINES")

add_subdirectory("Libraries")
add_subdirectory("Test")
add_subdirectory("Examples")
