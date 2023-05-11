cmake_minimum_required(VERSION 3.21)
project(Modules_Render)

set(CMAKE_CXX_STANDARD 20)

add_library(${PROJECT_NAME} STATIC
    "3D/Object3D.cpp"
    "3D/Camera3D.cpp"
    "3D/Fly_Camera3D.cpp"
    "3D/OpenGL_Object3D.cpp"
    "Shader/Fragment_OpenGL_Shader.cpp"
    "Shader/OpenGL_ShaderProgram.cpp"
    "Shader/Shader.cpp"
    "Shader/OpenGL_Shader.cpp"
    "Shader/Vertex_OpenGL_Shader.cpp"
    "Shader/Geometry_OpenGL_Shader.cpp"
    "Shader/Compute_OpenGL_Shader.cpp"
    "Texture/stb_implementation.cpp"
    "Texture/Texture.cpp"
    "Texture/ImageFile.cpp"
    "Texture/OpenGL_Texture.cpp"
    "Renderer/Renderer_OpenGL.cpp"
    "Renderer/Renderer.cpp"
)

find_package(OpenGL REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE Modules_GUI Modules_Generic Modules_Jobs fmt::fmt OpenGL::GL glfw glad::glad)
target_include_directories(${PROJECT_NAME} PRIVATE ${OPENGL_INCLUDE_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})