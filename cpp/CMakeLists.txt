cmake_minimum_required(VERSION 3.14)

project(message_processor)

set (CMAKE_CXX_STANDARD 14)

add_subdirectory(external/spdlog)
find_package(spdlog)

add_subdirectory(external/googletest)
find_package(gtest)

add_executable(message_processor src/main/main.cpp)
target_link_libraries(message_processor spdlog gtest)


