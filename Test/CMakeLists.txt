cmake_minimum_required(VERSION 3.5)

project(Ignosi.Test)

include(GoogleTest)

set(CMAKE_CXX_STANDARD 20)

add_executable(${PROJECT_NAME} "main.cpp")

add_compile_definitions(TEST_SOURCE_DIR=R"\(${CMAKE_CURRENT_SOURCE_DIR}\)")

find_package(GTest CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE GTest::gtest)

gtest_add_tests(TARGET ${PROJECT_NAME})
