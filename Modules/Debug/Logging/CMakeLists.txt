cmake_minimum_required(VERSION 3.21)
project("Modules_Debug_Logging")

set(CMAKE_CXX_STANDARD 20)

add_library(${PROJECT_NAME} STATIC
	"src/ILogManager.cpp"
	"src/Logger.cpp"
	"src/LogManager.cpp"
)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc)

find_package(spdlog CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE spdlog::spdlog spdlog::spdlog_header_only)