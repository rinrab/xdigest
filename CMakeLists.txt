cmake_minimum_required(VERSION 3.12)

project(xdigest LANGUAGES C ASM)

include(GNUInstallDirs)

file(
  STRINGS "xdigest/include/xdigest/xdigest.h" VERSION_STRINGS
  REGEX "#define (XDIG_VERSION_MAJOR|XDIG_VERSION_MINOR|XDIG_VERSION_PATCH)"
)

string(REGEX REPLACE ".*XDIG_VERSION_MAJOR +([0-9]+).*" "\\1" XDIG_VERSION_MAJOR ${VERSION_STRINGS})
string(REGEX REPLACE ".*XDIG_VERSION_MINOR +([0-9]+).*" "\\1" XDIG_VERSION_MINOR ${VERSION_STRINGS})
string(REGEX REPLACE ".*XDIG_VERSION_PATCH +([0-9]+).*" "\\1" XDIG_VERSION_PATCH ${VERSION_STRINGS})

set(XDIG_VERSION "${XDIG_VERSION_MAJOR}.${XDIG_VERSION_MINOR}.${XDIG_VERSION_PATCH}")

option(BUILD_SHARED_LIBS "Build using shared libraries" ON)
option(USE_ASM "Enable assembly implementation." ON)
option(ENABLE_TESTS "Compile test-suite" ON)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -Wall")
set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -Wa,--noexecstack")

set(ASMEXT S)

try_compile(
    DETECT_PLATFORM_RESULT
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/build/detect_platform.c
    OUTPUT_VARIABLE DETECT_PLATFORM_OUTPUT
)

string(REGEX MATCH "DETECTED: ([a-z0-9]+)" _ ${DETECT_PLATFORM_OUTPUT})
set(CONFIG ${CMAKE_MATCH_1})

if ("${CONFIG}" STREQUAL "unsupported")
    message(ERROR "Could not detect assembly architecture or one is not supported.")
endif()

message(STATUS "Detected configuration: ${CONFIG}")

add_library(xdigest
    xdigest/core/cpuid.c
    xdigest/core/armcap.c
    xdigest/core/version.c
    xdigest/core/mem_clr.c
    xdigest/sha/sha1.c
    xdigest/sha/sha256.c
    xdigest/sha/sha512.c
    xdigest/md5/md5_dgst.c
    xdigest/md5/md5_one.c
    xdigest/md4/md4_dgst.c
    xdigest/md4/md4_one.c
    xdigest/md2/md2_dgst.c
    xdigest/md2/md2_one.c
)

if (USE_ASM)
    target_sources(xdigest PRIVATE
        xdigest/core/asm/cpuid-${CONFIG}.${ASMEXT}
        xdigest/sha/asm/sha256-${CONFIG}.${ASMEXT}
        xdigest/sha/asm/sha512-${CONFIG}.${ASMEXT}
        xdigest/sha/asm/sha1-${CONFIG}.${ASMEXT}
        xdigest/md5/asm/md5-${CONFIG}.${ASMEXT}
    )
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
    "${CMAKE_CURRENT_SOURCE_DIR}/xdigest/include"
)

target_compile_definitions(xdigest PRIVATE
    SHA1_ASM
    SHA256_ASM
    SHA512_ASM
    MD5_ASM
    xdig_CPUID_OBJ
)

install(TARGETS xdigest)

file(GLOB public_headers "xdigest/include/xdigest/*.h")
install(FILES
    ${public_headers}
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/xdigest"
)

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
message(STATUS "  Build test suite ................ : TODO")
message(STATUS "  Install prefix: ................. : ${CMAKE_INSTALL_PREFIX}")
