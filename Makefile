out = out

ifeq ($(PREFIX),)
    PREFIX := /usr/local
endif

core_c_objects = src/core/cpuid.o src/core/ctype.o src/core/ebcdic.o
core_asm_objects = asm/elf/x86_64cpuid.o asm/elf/sha/sha1-x86_64.o

sha_c_objects = src/sha/sha1.o src/sha/sha256.o src/sha/sha512.o
sha_asm_objects = asm/elf/sha/sha256-x86_64.o asm/elf/sha/sha512-x86_64.o

md5_c_objects = src/md5/md5_dgst.o src/md5/md5_one.o
md5_asm_objects =

c_objects = $(core_c_objects) $(sha_c_objects) $(md5_c_objects)
asm_objects = $(core_asm_objects) $(sha_asm_objects) $(md5_asm_objects)

objects = $(asm_objects) $(c_objects)

CC = cc
ASSEMBLER = $(CC)

CFLAGS = -O3 -Wall -Iinclude -DSHA1_ASM -DSHA256_ASM -DSHA512_ASM -Dxhash_CPUID_OBJ
ASMFLAGS = -Wa,--noexecstack

all: libxhash.so
.PHONY: all

libxhash_core.a: $(core_c_objects) $(core_asm_objects)
	mkdir -p $(@D) && ar rcs $@ $^

libxhash_sha.a: $(sha_c_objects) $(sha_asm_objects)
	mkdir -p $(@D) && ar rcs $@ $^

libxhash_md5.a: $(md5_c_objects) $(md5_asm_objects)
	mkdir -p $(@D) && ar rcs $@ $^

libxhash.so: libxhash_core.a libxhash_sha.a libxhash_md5.a
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -shared -o $@ -L. -Wl,--whole-archive $^ -Wl,--no-whole-archive

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

install: all
	install -d $(DESTDIR)$(PREFIX)/lib/
	install -m 644 libxhash.so $(DESTDIR)$(PREFIX)/lib/
	install -d $(DESTDIR)$(PREFIX)/include/
	install -d $(DESTDIR)$(PREFIX)/include/xhash
	install -m 644 include/xhash/*.h $(DESTDIR)$(PREFIX)/include/xhash

rebuild: clean all

# TODO: checksum
OPENSSL_VERSION = 3.6.0

openssl-$(OPENSSL_VERSION).zip:
	wget https://github.com/openssl/openssl/archive/refs/tags/openssl-$(OPENSSL_VERSION).zip

openssl: openssl-$(OPENSSL_VERSION).zip
	rm -rf openssl openssl-openssl-$(OPENSSL_VERSION)
	unzip openssl-$(OPENSSL_VERSION).zip
	mv openssl-openssl-$(OPENSSL_VERSION) openssl

openssl-clean:
	rm -f openssl-$(OPENSSL_VERSION)*.zip
	rm -rf openssl
	rm -rf openssl-$(OPENSSL_VERSION)

generate: openssl
	python3 generate.py
