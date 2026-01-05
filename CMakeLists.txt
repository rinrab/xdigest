project(xhash LANGUAGES C ASM)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(CMAKE_ASM_COMPILER gcc)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -Wall")
set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -Wa,--noexecstack")

add_library(xhash
  src/sha/sha1.c
  src/sha/sha256.c
  src/sha/sha512.c
  asm/elf/sha/asm/sha1-x86_64.S
  # asm/elf/sha/asm/sha256-x86_64.S
  asm/elf/sha/asm/sha512-x86_64.S
  asm/elf/x86_64cpuid.S
  src/cpuid.c
  src/ctype.c
  src/ebcdic.c
)

target_include_directories(xhash PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_compile_definitions(xhash PRIVATE
  SHA1_ASM
  SHA256_ASM
  SHA512_ASM
  OPENSSL_CPUID_OBJ
)

add_executable(test_xhash tests/test_xhash.c)
target_link_libraries(test_xhash xhash)

