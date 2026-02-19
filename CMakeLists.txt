#
# CMakeLists.txt: configuration for cmake build system
#
# ====================================================================
# Copyright 2026, Timofei Zhakov
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
# ====================================================================
#

cmake_minimum_required(VERSION 3.12)

project(xdigest LANGUAGES C)

include(GNUInstallDirs)
include(build/target_exports.cmake)

# Detect architecture
try_compile(
    DETECT_PLATFORM_RESULT
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/build/detect_platform.c
    OUTPUT_VARIABLE DETECT_PLATFORM_OUTPUT
)

string(REGEX MATCH "DETECTED: ([a-z0-9]+)" _ ${DETECT_PLATFORM_OUTPUT})
set(CONFIG ${CMAKE_MATCH_1})

if ("${CONFIG}" STREQUAL "unsupported")
    set(ASM_SUPPORTED OFF)
else()
    set(ASM_SUPPORTED ON)
endif()

# options
include(CMakeDependentOption)

option(BUILD_SHARED_LIBS "Build using shared libraries" ON)
cmake_dependent_option(USE_ASM "Enable assembly implementation." ON ASM_SUPPORTED OFF)
option(ENABLE_TESTS "Compile test-suite" ON)

if ("${CONFIG}" STREQUAL "unsupported")
    message(ERROR "Could not detect assembly architecture or one is not supported.")
endif()

if (USE_ASM)
    if (CONFIG MATCHES "win*.")
        enable_language(ASM_NASM)
        set(ASMEXT asm)
    else()
        enable_language(ASM)
        set(ASMEXT S)
        set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -Wa,--noexecstack")
    endif()
endif()

file(
  STRINGS "xdigest/include/xdigest/xdigest.h" VERSION_STRINGS
  REGEX "#define (XDIG_VERSION_MAJOR|XDIG_VERSION_MINOR|XDIG_VERSION_PATCH)"
)

string(REGEX REPLACE ".*XDIG_VERSION_MAJOR +([0-9]+).*" "\\1" XDIG_VERSION_MAJOR ${VERSION_STRINGS})
string(REGEX REPLACE ".*XDIG_VERSION_MINOR +([0-9]+).*" "\\1" XDIG_VERSION_MINOR ${VERSION_STRINGS})
string(REGEX REPLACE ".*XDIG_VERSION_PATCH +([0-9]+).*" "\\1" XDIG_VERSION_PATCH ${VERSION_STRINGS})

set(XDIG_VERSION "${XDIG_VERSION_MAJOR}.${XDIG_VERSION_MINOR}.${XDIG_VERSION_PATCH}")

add_library(xdigest
    xdigest/core/cpuid.c
    xdigest/core/armcap.c
    xdigest/core/riscvcap.c
    xdigest/core/version.c
    xdigest/core/mem_clr.c
    xdigest/sha/sha1.c
    xdigest/sha/sha256.c
    xdigest/sha/sha512.c
    xdigest/sha/sha_riscv.c
    xdigest/md5/md5_dgst.c
    xdigest/md5/md5_one.c
    xdigest/md5/md5_riscv.c
    xdigest/md4/md4_dgst.c
    xdigest/md4/md4_one.c
    xdigest/md2/md2_dgst.c
    xdigest/md2/md2_one.c
)

set_target_properties(xdigest PROPERTIES
    VERSION "${XDIG_VERSION}"
    SOVERSION "${XDIG_VERSION_MAJOR}"
)

if (USE_ASM)
    if (CONFIG MATCHES "riscv")
        target_sources(xdigest PRIVATE
            xdigest/core/asm/cpuid-${CONFIG}.${ASMEXT}
            xdigest/sha/asm/sha512-zbb-${CONFIG}.${ASMEXT}
            xdigest/sha/asm/sha512-zvkb-zvknhb-${CONFIG}.${ASMEXT}
            xdigest/sha/asm/sha256-zbb-${CONFIG}.${ASMEXT}
            xdigest/sha/asm/sha256-zvkb-zvknha_or_zvknhb-${CONFIG}.${ASMEXT}
            xdigest/md5/asm/md5-zbb-${CONFIG}.${ASMEXT}
        )

        target_compile_definitions(xdigest PRIVATE
            SHA256_ASM
            SHA512_ASM
            MD5_ASM
            xdig_CPUID_OBJ
        )
    else()
        target_sources(xdigest PRIVATE
            xdigest/core/asm/cpuid-${CONFIG}.${ASMEXT}
            xdigest/sha/asm/sha256-${CONFIG}.${ASMEXT}
            xdigest/sha/asm/sha512-${CONFIG}.${ASMEXT}
            xdigest/sha/asm/sha1-${CONFIG}.${ASMEXT}
            xdigest/md5/asm/md5-${CONFIG}.${ASMEXT}
        )

        target_compile_definitions(xdigest PRIVATE
            SHA1_ASM
            SHA256_ASM
            SHA512_ASM
            MD5_ASM
            xdig_CPUID_OBJ
        )
    endif()
else()
    target_compile_definitions(xdigest PRIVATE NO_ASM)
endif()

if (ENABLE_TESTS)
    enable_testing()

    add_executable(test_xdigest
        tests/test_xdigest.c
        tests/sha_test.c
    )
    target_link_libraries(test_xdigest PRIVATE xdigest)
    target_compile_definitions(test_xdigest PRIVATE XDIG)
    add_test(
        NAME test_xdigest
        COMMAND test_xdigest
    )
endif()

target_include_directories(xdigest PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/xdigest/include/xdigest>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/xdigest>"
)

target_include_directories(xdigest PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/xdigest/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/xdigest/core"
)

# Installation
install(
    TARGETS xdigest
    EXPORT xdigest-config
)

include(CMakePackageConfigHelpers)

install(
    EXPORT xdigest-config
    NAMESPACE xdigest::
    FILE "xdigest-config.cmake"
    DESTINATION "lib/cmake/xdigest"
)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/xdigest-config-version.cmake"
    VERSION ${XDIG_VERSION}
    COMPATIBILITY SameMajorVersion
)
install(
    FILES "${CMAKE_CURRENT_BINARY_DIR}/xdigest-config-version.cmake"
    DESTINATION "lib/cmake/xdigest"
)

file(GLOB public_headers "xdigest/include/xdigest/*.h")
install(FILES
    ${public_headers}
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/xdigest"
)
target_exports(xdigest ${public_headers})

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/build/xdigest.pc.in"
    "${CMAKE_CURRENT_BINARY_DIR}/xdigest.pc" @ONLY
)

install(
    FILES "${CMAKE_CURRENT_BINARY_DIR}/xdigest.pc"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/pkgconfig"
)

message(STATUS "Configuration summary:")
message(STATUS "  Version ......................... : ${XDIG_VERSION}")
message(STATUS "  Build type ...................... : ${CMAKE_BUILD_TYPE}")
message(STATUS "  Assembly configuration .......... : ${CONFIG}")
message(STATUS "  Using Assembly .................. : ${USE_ASM}")
message(STATUS "  Build test suite ................ : ${ENABLE_TESTS}")
message(STATUS "  Install prefix: ................. : ${CMAKE_INSTALL_PREFIX}")
