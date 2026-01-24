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
    CFLAGS += -DNO_ASM
else
    CFLAGS += -Dxdig_CPUID_OBJ
    CFLAGS += -DMD5_ASM
    CFLAGS += -DSHA1_ASM -DSHA256_ASM -DSHA512_ASM
endif

OBJEXT = .o
include Makefile.inc

objects = $(c_objects)
ifndef NO_ASM
    objects += $(asm_objects)
endif

sofiles = \
	libxdigest.so.$(VERSION) \
	libxdigest.so.$(SONAME) \
	libxdigest.so

all: libxdigest.so
.PHONY: all

libxdigest.so.$(VERSION): $(objects)
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -shared -o $@ $^

libxdigest.so.$(SONAME): libxdigest.so.$(VERSION)
	ln -s $^ $@

libxdigest.so: libxdigest.so.$(SONAME)
	ln -s $^ $@

%.o: %.S
	mkdir -p $(@D) && $(ASSEMBLER) -c $^ -o $@ $(CFLAGS) $(ASMFLAGS)

%.o: %.c
	mkdir -p $(@D) && $(CC) -c $^ -o $@ $(CFLAGS)

test_xdigest: tests/test_xdigest.c tests/sha_test.c libxdigest.so
	mkdir -p $(@D) && $(CC) $^ -o $@ $(CFLAGS) -DXDIG -Lxdigest.so

test: test_xdigest
	export "LD_LIBRARY_PATH=$(CURDIR):$(LD_LIBRARY_PATH)" && ./test_xdigest

clean:
	find . -type f -name '*.o' -delete
	rm -f $(sofiles)
	find . -type f -name '*.a' -delete
	find . -type f -name '*.svnpatch.rej' -delete

install: all
	install -d $(DESTDIR)$(PREFIX)/lib/
	install -m 644 $(sofiles) $(DESTDIR)$(PREFIX)/lib/
	install -d $(DESTDIR)$(PREFIX)/include/
	install -d $(DESTDIR)$(PREFIX)/include/xdigest
	install -m 644 include/xdigest/*.h $(DESTDIR)$(PREFIX)/include/xdigest

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
