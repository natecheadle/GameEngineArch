cmake_minimum_required(VERSION 3.21)
project(Module_Unit_Tests)

set(CMAKE_CXX_STANDARD 20)

add_executable(${PROJECT_NAME} 
    "main.cpp"
    "StaticMemoryBlock_Tests.cpp"
    "Jobs_Tests.cpp" 
    "Logging_Tests.cpp"
    "Messaging_Tests.cpp"
    "StaticMap_Tests.cpp"
)

find_package(GTest CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE Modules GTest::gtest)