cmake_minimum_required(VERSION 3.21)

project(Ignosi.Test)

include(GoogleTest)

set(CMAKE_CXX_STANDARD 20)

add_executable(
  ${PROJECT_NAME}
  "main.cpp"
  "LinearAlgebra_Tests.cpp"
  "Units_Tests.cpp"
  "StringLiteral_Tests.cpp"
  "Event_Tests.cpp"
  "Property_Tests.cpp"
  "ObjectPool_Tests.cpp"
  "Queue_Tests.cpp"
  "Stack_Tests.cpp"
  "RingBuffer_Tests.cpp"
  "ECS/System_Tests.cpp"
  "ECS/Entity_Tests.cpp"
  "ECS/ResourceManager_Tests.cpp")

find_package(GTest CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(Boost REQUIRED COMPONENTS atomic)

target_link_libraries(
  ${PROJECT_NAME}
  PRIVATE GTest::gtest
          glm::glm
          Ignosi.Libraries.Math
          Ignosi.Libraries.Generic
          Ignosi.Libraries.Container
          Ignosi.Libraries.ECS
          Boost::boost
          Boost::atomic)

gtest_add_tests(TARGET ${PROJECT_NAME})
