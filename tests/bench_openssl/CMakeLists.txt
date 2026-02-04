cmake_minimum_required(VERSION 3.12)

project(bench_openssl LANGUAGES C)

add_executable(bbench_openssl ench_openssl.c)

find_package(OpenSSL REQUIRED)
target_link_libraries(bench_openssl PRIVATE OpenSSL::Crypto)
