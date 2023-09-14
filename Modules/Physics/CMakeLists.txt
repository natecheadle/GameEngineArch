cmake_minimum_required(VERSION 3.21)
project(Modules_Physics)

set(CMAKE_CXX_STANDARD 20)

add_library(${PROJECT_NAME} STATIC
    "PhysicsSystem.cpp"
    "RigidBody2D.cpp"
    "HitShape.cpp"
    "HitRectangle.cpp"
    "HitCircle.cpp"
    "KinematicData.cpp"
)

target_link_libraries(${PROJECT_NAME} PUBLIC Modules_ECS Modules_Messaging)
target_link_libraries(${PROJECT_NAME} PRIVATE  Modules_Generic )
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})