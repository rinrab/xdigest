out = out

ifeq ($(PREFIX),)
    PREFIX := /usr/local
endif

CC = cc
ASSEMBLER = $(CC)

CFLAGS += -O3 -Wall -Iinclude
CFLAGS += -Dxhash_CPUID_OBJ -DSHA1_ASM -DSHA256_ASM -DSHA512_ASM -DMD5_ASM

ASMFLAGS = -Wa,--noexecstack

ifeq ($(ARCH),)
	ARCH = x86_64
endif

ifeq ($(ARCH), x86_64)
CONFIG = linux64
CFLAGS += -m64
else ifeq ($(ARCH), x86)
CONFIG = linux32
CFLAGS += -m32
else
$(error invalid architecute: "$(ARCH)")
endif

core_c_objects = \
	src/core/cpuid.o \
	src/core/ebcdic.o
core_asm_objects = \
	src/core/asm/cpuid-$(CONFIG).o

sha_c_objects = \
	src/sha/sha1.o \
	src/sha/sha256.o \
	src/sha/sha512.o
sha_asm_objects = \
	src/sha/asm/sha256-$(CONFIG).o \
	src/sha/asm/sha512-$(CONFIG).o \
	src/sha/asm/sha1-$(CONFIG).o

md5_c_objects = \
	src/md5/md5_dgst.o \
	src/md5/md5_one.o
md5_asm_objects = \
	src/md5/asm/md5-$(CONFIG).o

md4_c_objects = \
	src/md4/md4_dgst.o \
	src/md4/md4_one.o
md4_asm_objects =

md2_c_objects = \
	src/md2/md2_dgst.o \
	src/md2/md2_one.o
md2_asm_objects =

c_objects = \
	$(core_c_objects) \
	$(sha_c_objects) \
	$(md5_c_objects) \
	$(md4_c_objects) \
	$(md2_c_objects)

asm_objects = \
	$(core_asm_objects) \
	$(sha_asm_objects) \
	$(md5_asm_objects) \
	$(md4_asm_objects) \
	$(md2_asm_objects)

libs = \
	libxhash_core.a \
	libxhash_sha.a \
	libxhash_md5.a \
	libxhash_md4.a \
	libxhash_md2.a

objects = $(asm_objects) $(c_objects)

all: libxhash.so
.PHONY: all

libxhash_core.a: $(core_c_objects) $(core_asm_objects)
	mkdir -p $(@D) && ar rcs $@ $^

libxhash_sha.a: $(sha_c_objects) $(sha_asm_objects)
	mkdir -p $(@D) && ar rcs $@ $^

libxhash_md5.a: $(md5_c_objects) $(md5_asm_objects)
	mkdir -p $(@D) && ar rcs $@ $^

libxhash_md4.a: $(md4_c_objects) $(md4_asm_objects)
	mkdir -p $(@D) && ar rcs $@ $^

libxhash_md2.a: $(md2_c_objects) $(md2_asm_objects)
	mkdir -p $(@D) && ar rcs $@ $^

libxhash.so: $(libs)
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
	find . -type f -name '*.o' -delete
	find . -type f -name '*.so' -delete
	find . -type f -name '*.a' -delete
	find . -type f -name '*.svnpatch.rej' -delete

install: all
	install -d $(DESTDIR)$(PREFIX)/lib/
	install -m 644 libxhash.so $(DESTDIR)$(PREFIX)/lib/
	install -m 644 $(libs) $(DESTDIR)$(PREFIX)/lib/
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
