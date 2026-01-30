cmake_minimum_required(VERSION 3.12)

project(test_cmake_module LANGUAGES C)

add_executable(test_cmake_module main.c)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../build")
find_package(XDIGEST REQUIRED)
target_link_libraries(test_cmake_module PRIVATE XDIGEST::XDIGEST)
