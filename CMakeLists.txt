cmake_minimum_required(VERSION 3.12)

project(xdigest LANGUAGES C ASM)

option(BUILD_SHARED_LIBS "Build using shared libraries" ON)
option(NO_ASM "Dissable assembly implementation." OFF)

set(CMAKE_ASM_COMPILER gcc)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -Wall")
set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -Wa,--noexecstack")

set(CONFIG linux64)
set(ASMEXT S)

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
    xdigest/core/asm/cpuid-${CONFIG}.${ASMEXT}
    xdigest/sha/asm/sha256-${CONFIG}.${ASMEXT}
    xdigest/sha/asm/sha512-${CONFIG}.${ASMEXT}
    xdigest/sha/asm/sha1-${CONFIG}.${ASMEXT}
    xdigest/md5/asm/md5-${CONFIG}.${ASMEXT}
)

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
