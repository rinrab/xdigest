out = out

ifeq ($(PREFIX),)
    PREFIX := /usr/local
endif

include Version.inc

CFLAGS += -O3 -Wall -Iinclude -Isrc/core
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
else ifeq ($(ARCH), aarch64)
    CONFIG = linuxaarch64
    CFLAGS += -march=armv8-a+crypto
    CROSS_COMPILE ?= aarch64-linux-gnu-
else
    $(error invalid architecute: "$(ARCH)")
endif

CC = $(CROSS_COMPILE)gcc
ASSEMBLER = $(CC)

ifdef NO_ASM
    CFLAGS += NO_ASM
else
    CFLAGS += -Dxhash_CPUID_OBJ
    CFLAGS += -DMD5_ASM
    CFLAGS += -DSHA1_ASM -DSHA256_ASM -DSHA512_ASM
endif

c_objects = \
    src/core/cpuid.o \
    src/core/armcap.o \
    src/core/version.o \
    src/core/ebcdic.o \
    src/core/mem_clr.o \
    src/sha/sha1.o \
    src/sha/sha256.o \
    src/sha/sha512.o \
    src/md5/md5_dgst.o \
    src/md5/md5_one.o \
    src/md4/md4_dgst.o \
    src/md4/md4_one.o \
    src/md2/md2_dgst.o \
    src/md2/md2_one.o

asm_objects = \
    src/core/asm/cpuid-$(CONFIG).o \
    src/sha/asm/sha256-$(CONFIG).o \
    src/sha/asm/sha512-$(CONFIG).o \
    src/sha/asm/sha1-$(CONFIG).o \
    src/md5/asm/md5-$(CONFIG).o

objects = $(c_objects)
ifndef NO_ASM
    objects += $(asm_objects)
endif

sofiles = \
	libxhash.so.$(VERSION) \
	libxhash.so.$(SONAME) \
	libxhash.so

all: libxhash.so
.PHONY: all

libxhash.so.$(VERSION): $(objects)
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -shared -o $@ $^

libxhash.so.$(SONAME): libxhash.so.$(VERSION)
	ln -s $^ $@

libxhash.so: libxhash.so.$(SONAME)
	ln -s $^ $@

%.o: %.S
	mkdir -p $(@D) && $(ASSEMBLER) -c $^ -o $@ $(CFLAGS) $(ASMFLAGS)

%.o: %.c
	mkdir -p $(@D) && $(CC) -c $^ -o $@ $(CFLAGS)

test_xhash: tests/test_xhash.c tests/sha_test.c libxhash.so
	mkdir -p $(@D) && $(CC) $^ -o $@ $(CFLAGS) -DXHASH -Lxhash.so

test: test_xhash
	export "LD_LIBRARY_PATH=$(CURDIR):$(LD_LIBRARY_PATH)" && ./test_xhash

clean:
	find . -type f -name '*.o' -delete
	rm -f $(sofiles)
	find . -type f -name '*.a' -delete
	find . -type f -name '*.svnpatch.rej' -delete

install: all
	install -d $(DESTDIR)$(PREFIX)/lib/
	install -m 644 $(sofiles) $(DESTDIR)$(PREFIX)/lib/
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
