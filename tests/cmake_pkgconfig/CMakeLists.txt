cmake_minimum_required(VERSION 3.12)

project(test_cmake_pkgconfig LANGUAGES C)

add_executable(test_cmake_pkgconfig main.c)

find_package(PkgConfig REQUIRED)
pkg_check_modules(xdigest REQUIRED IMPORTED_TARGET xdigest)
target_link_libraries(test_cmake_pkgconfig PRIVATE PkgConfig::xdigest)
