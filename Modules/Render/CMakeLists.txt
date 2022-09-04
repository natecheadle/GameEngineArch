cmake_minimum_required(VERSION 3.21)
project("Modules_Render")

set(CMAKE_CXX_STANDARD 20)

add_library(${PROJECT_NAME} STATIC
    "Renderer.cpp"
    "Shader/Shader.cpp"
    "Shader/BGFX_Shader.cpp"
    "3D/Camera/Camera3D.cpp"
    "3D/Camera/Fly_Camera3D.cpp"
    "3D/Object3D.cpp"
    "Matrix4x4.cpp"
    "StateMachine/RendererSM.cpp"
    "StateMachine/States/RendererSM_Initializing.cpp"
    "StateMachine/States/RendererSM_Processing_ConfiguringView.cpp"
    "StateMachine/States/RendererSM_Processing_SubmittingObjects.cpp"
    "StateMachine/States/RendererSM_Processing_WaitingForFrame.cpp"
    "StateMachine/States/RendererSM_ShuttingDown.cpp")
    
find_package(bgfx CONFIG REQUIRED)
find_package(Boost REQUIRED)

target_link_libraries(${PROJECT_NAME} PUBLIC bgfx::bgfx)
target_link_libraries(${PROJECT_NAME} PRIVATE Modules_GUI Modules_Jobs bgfx::bx bgfx::iqa bgfx::astc Modules_Generic Boost::boost)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})