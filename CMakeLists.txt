cmake_minimum_required(VERSION 3.21)
project(GameEngine)

enable_testing()

add_compile_definitions(
    PUBLIC
      $<$<CONFIG:Debug>:DEBUG>
      "_USE_MATH_DEFINES"
)

add_subdirectory("Modules")
add_subdirectory("Test")
add_subdirectory("App")
