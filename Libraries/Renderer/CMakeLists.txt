cmake_minimum_required(VERSION 3.21)

set(CMAKE_CXX_STANDARD 20)

project(Ignosi.Libraries.Renderer)

find_package(OpenGL REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)

add_library(
  ${PROJECT_NAME} STATIC
  "IWindow.cpp"
  "GLFW/GLFW_Window.cpp"
  "IRenderer.cpp"
  "OpenGL/OpenGL_Renderer.cpp"
  "OpenGL/OpenGL_VertexBuffer.cpp"
  "OpenGL/Shader/Compute_OpenGL_Shader.cpp"
  "OpenGL/Shader/Fragment_OpenGL_Shader.cpp"
  "OpenGL/Shader/Geometry_OpenGL_Shader.cpp"
  "OpenGL/Shader/OpenGL_Shader.cpp"
  "OpenGL/Shader/OpenGL_ShaderProgram.cpp"
  "OpenGL/Shader/Vertex_OpenGL_Shader.cpp"
  "OpenGL/Texture/OpenGL_Texture.cpp"
  "Shader/Preprocessor.cpp"
  "Shader/Shader.cpp"
  "Texture/ImageFile.cpp"
  "Texture/stb_implementation.cpp"
  "Texture/Texture.cpp"
  "Camera/Camera.cpp"
  "Camera/Camera2D.cpp"
  "Camera/Fly_Camera.cpp"
  "Objects/Mesh.cpp")

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${OPENGL_INCLUDE_DIR})

target_link_libraries(${PROJECT_NAME} PRIVATE fmt::fmt OpenGL::GL glfw
                                              glad::glad)
target_link_libraries(${PROJECT_NAME} PUBLIC Ignosi.Libraries.Math
                                             Ignosi.Libraries.ECS)
