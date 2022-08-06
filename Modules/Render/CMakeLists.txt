cmake_minimum_required(VERSION 3.21)
project("Modules_Render")

set(CMAKE_CXX_STANDARD 20)

add_library(${PROJECT_NAME} STATIC
    "src/Renderer.cpp"
    "Shader/src/Shader.cpp"
    "Shader/src/BGFX_Shader.cpp"
    "3D/Camera3D.cpp"
    "3D/Object3D.cpp"
    "src/Matrix4x4.cpp")
    
find_package(bgfx CONFIG REQUIRED)

target_link_libraries(${PROJECT_NAME} PUBLIC bgfx::bgfx)
target_link_libraries(${PROJECT_NAME} PRIVATE Modules_GUI Modules_Jobs bgfx::bx bgfx::iqa bgfx::astc Modules_Generic)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc ${CMAKE_CURRENT_SOURCE_DIR}/Shader/inc)