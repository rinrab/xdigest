cmake_minimum_required(VERSION 3.12)

project(test_cmake_config LANGUAGES C)

add_executable(test_cmake_config main.c)

find_package(xdigest CONFIG REQUIRED)
target_link_libraries(test_cmake_config PRIVATE xdigest::xdigest)
