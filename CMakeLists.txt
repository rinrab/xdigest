project(xhash LANGUAGES C ASM)

set(CMAKE_ASM_COMPILER gcc)

add_library(xhash
  src/sha/sha256.c
  src/sha/sha512.c
  asm/elf/sha/asm/sha1-x86_64.S
  # asm/elf/sha/asm/sha256-x86_64.S
  asm/elf/sha/asm/sha512-x86_64.S
)

target_include_directories(xhash PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

