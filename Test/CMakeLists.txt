cmake_minimum_required(VERSION 3.21)
project(Module_Unit_Tests)
include(GoogleTest)

set(CMAKE_CXX_STANDARD 20)

add_executable(${PROJECT_NAME} 
    "main.cpp"
    "StaticMemoryBlock_Tests.cpp"
    "Jobs_Tests.cpp" 
    "Logging_Tests.cpp"
    "Messaging_Tests.cpp"
    "StaticMap_Tests.cpp"
    "ECS_Tests.cpp"
    "LinearAlg_Tests.cpp"
    "Units_Tests.cpp"
    "PreProcessor_Tests.cpp"
    "Physics_Tests.cpp"
)

add_compile_definitions(TEST_SOURCE_DIR=R"\(${CMAKE_CURRENT_SOURCE_DIR}\)")

find_package(glm CONFIG REQUIRED)
find_package(GTest CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE Modules GTest::gtest  glm::glm)

gtest_add_tests(TARGET ${PROJECT_NAME})