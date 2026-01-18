out = out
prefix = /usr/local

asm_objects = asm/elf/x86_64cpuid.o asm/elf/sha/sha1-x86_64.o \
	asm/elf/sha/sha256-x86_64.o asm/elf/sha/sha512-x86_64.o

c_objects = src/core/cpuid.o src/core/ctype.o src/core/ebcdic.o src/sha/sha1.o \
	src/sha/sha256.o src/sha/sha512.o

objects = $(asm_objects) $(c_objects)

CC = cc
ASSEMBLER = $(CC)

CFLAGS = -O3 -Wall -Iinclude -DSHA1_ASM -DSHA256_ASM -DSHA512_ASM -Dxhash_CPUID_OBJ
ASMFLAGS = -Wa,--noexecstack

all: libxhash.so
.PHONY: all

libxhash.so: $(objects)
	mkdir -p $(@D) && $(CC) -shared $^ -o $@ $(CFLAGS)

$(asm_objects): %.o: %.S
	mkdir -p $(@D) && $(ASSEMBLER) -c $^ -o $@ $(CFLAGS) $(ASMFLAGS)

$(c_objects): %.o: %.c
	mkdir -p $(@D) && $(CC) -c $^ -o $@ $(CFLAGS)

test_xhash: tests/test_xhash.c tests/sha_test.c libxhash.so
	mkdir -p $(@D) && $(CC) $^ -o $@ $(CFLAGS) -DXHASH -Lxhash.so

test: test_xhash
	export "LD_LIBRARY_PATH=$(CURDIR):$(LD_LIBRARY_PATH)" && ./test_xhash

clean:
	rm -f $(objects) libxhash.so test_xhash

rebuild: clean all

