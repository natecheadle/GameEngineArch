cmake_minimum_required(VERSION 3.21)

project(Ignosi.Examples.HelloTriangle)

set(CMAKE_CXX_STANDARD 23)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE Ignosi.Libraries.Renderer)

add_compile_definitions(OUT_DIR="${CMAKE_CURRENT_BINARY_DIR}")

ADD_CUSTOM_TARGET(
        copy_assets
        ${CMAKE_COMMAND}
        -E copy_directory_if_different
        ${CMAKE_CURRENT_LIST_DIR}/assets
        ${CMAKE_CURRENT_BINARY_DIR}/assets
        COMMENT "Copying assets to output directory" VERBATIM 
        )

ADD_DEPENDENCIES(${PROJECT_NAME} copy_assets)
