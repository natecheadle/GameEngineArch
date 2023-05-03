cmake_minimum_required(VERSION 3.21)
project(RenderTestApp)

set(CMAKE_CXX_STANDARD 20)

add_executable(${PROJECT_NAME} 
    "main.cpp"
    "Shader/FragmentShader.cpp"
    "Shader/ShaderProgram.cpp"
    "Shader/Shader.cpp"
    "Shader/VertexShader.cpp"
    "Shader/GeometryShader.cpp"
    "Shader/ComputeShader.cpp"
)

add_compile_definitions(SHADER_DIR=R"\(${CMAKE_CURRENT_SOURCE_DIR}\)")

find_package(OpenGL REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE Modules OpenGL::GL glfw glad::glad)
target_include_directories(${PROJECT_NAME} PRIVATE ${OPENGL_INCLUDE_DIR})