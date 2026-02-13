#
# Makefile: configuration for make build system
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

out = out

include build/Version.inc
include build/rules.unix

CONFIG ?= linux64

ifeq ($(CONFIG), linux64)
    ARCH = -m64
else ifeq ($(CONFIG), linux32)
    ARCH = -m32
else ifeq ($(CONFIG), linuxaarch64)
    ARCH = -march=armv8-a+crypto
    CROSS_COMPILE ?= aarch64-linux-gnu-
else ifeq ($(CONFIG), macosx64)
    ARCH = -arch x86_64
else ifeq ($(CONFIG), macosxaarch64)
    ARCH = -arch arm64
else
    $(error invalid architecute: "$(CONFIG)")
endif

ASMFLAGS += -Wa,--noexecstack
CFLAGS += -O3 -Wall
CC = $(CROSS_COMPILE)gcc

COMPILE = $(CC) -c $(CFLAGS) $(ARCH) -o
ASSEMBLER = $(CC) -c $(ASMFLAGS) $(ARCH) -o

LINK_SHARED = $(CC) -shared $(ARCH) -o
LINK_PROGRAM = $(CC) $(ARCH) -o

ifdef NO_ASM
    DEFS += -DNO_ASM
else
    DEFS += -Dxdig_CPUID_OBJ
    DEFS += -DMD5_ASM
    DEFS += -DSHA1_ASM -DSHA256_ASM -DSHA512_ASM
endif

INCS += -Ixdigest/include -Ixdigest/include/xdigest -Ixdigest -Ixdigest/core

c_objects = \
    xdigest/core/cpuid$(OBJEXT) \
    xdigest/core/armcap$(OBJEXT) \
    xdigest/core/version$(OBJEXT) \
    xdigest/core/mem_clr$(OBJEXT) \
    xdigest/sha/sha1$(OBJEXT) \
    xdigest/sha/sha256$(OBJEXT) \
    xdigest/sha/sha512$(OBJEXT) \
    xdigest/md5/md5_dgst$(OBJEXT) \
    xdigest/md5/md5_one$(OBJEXT) \
    xdigest/md4/md4_dgst$(OBJEXT) \
    xdigest/md4/md4_one$(OBJEXT) \
    xdigest/md2/md2_dgst$(OBJEXT) \
    xdigest/md2/md2_one$(OBJEXT)

asm_objects = \
    xdigest/core/asm/cpuid-$(CONFIG)$(OBJEXT) \
    xdigest/sha/asm/sha256-$(CONFIG)$(OBJEXT) \
    xdigest/sha/asm/sha512-$(CONFIG)$(OBJEXT) \
    xdigest/sha/asm/sha1-$(CONFIG)$(OBJEXT) \
    xdigest/md5/asm/md5-$(CONFIG)$(OBJEXT)
objects = $(c_objects)

ifndef NO_ASM
    objects += $(asm_objects)
endif

sofiles = \
	xdigest/libxdigest.so.$(VERSION) \
	xdigest/libxdigest.so.$(SONAME) \
	xdigest/libxdigest.so

xdigest/libxdigest.so.$(VERSION): $(objects)
	$(MKDIR) $(@D)
	$(LINK_SHARED) $@ $^

xdigest/libxdigest.so.$(SONAME): xdigest/libxdigest.so.$(VERSION)
	$(LN) libxdigest.so.$(VERSION) $@

xdigest/libxdigest.so: xdigest/libxdigest.so.$(SONAME)
	$(LN) libxdigest.so.$(SONAME) $@

$(asm_objects): %.o: %.S
	mkdir -p $(@D) && $(ASSEMBLER) $@ $^ $(DEFS) $(INCS)

$(c_objects): %.o: %.c
	mkdir -p $(@D) && $(COMPILE) $@ $^ $(DEFS) $(INCS)

install-shared: $(sofiles)
	install -d $(prefix)/lib/
	install -m 644 $(sofiles) $(prefix)/lib/

install-headers:
	install -d $(prefix)/include/
	install -d $(prefix)/include/xdigest
	install -m 644 xdigest/include/xdigest/*.h $(prefix)/include/xdigest

MAKE_FLAGS = \
	 "ASSEMBLER=$(ASSEMBLER)" \
	 "COMPILE=$(COMPILE)" \
	 "LINK_SHARED=$(LINK_SHARED)" \
	 "CONFIG=$(CONFIG)" \
	 "prefix=$(prefix)"

all: $(sofiles)

clean:
	@find . -name '*.rej' | xargs $(RM)
	@find . -name '*.o' | xargs $(RM)
	@find . -name '*.so*' | xargs $(RM)
	$(RM) test_xdigest example
	$(RM) $(sofiles)
	$(RM) $(objects)

install: install-shared install-headers

rebuild: clean all

%.o: %.c
	mkdir -p $(@D)
	$(COMPILE) $@ $^ -DXDIG -Ixdigest/include/xdigest

test_xdigest: tests/test_xdigest.o tests/sha_test.o xdigest/libxdigest.so
	$(MKDIR) $(@D)
	$(LINK_PROGRAM) $@ $^

example: tests/example.o xdigest/libxdigest.so
	$(MKDIR) $(@D)
	$(LINK_PROGRAM) $@ $^

test: test_xdigest
	export "LD_LIBRARY_PATH=$(CURDIR)/xdigest:$(LD_LIBRARY_PATH)"
	export "DYLD_LIBRARY_PATH=$(CURDIR)/xdigest:$(DYLD_LIBRARY_PATH)"
	./test_xdigest

VCPKG_TRIPLET ?= x64-linux

ifdef NO_ASM
    VCPKG_FEATURES ?= core
else
    VCPKG_FEATURES ?= core,asm
endif

test-vcpkg:
	vcpkg remove xdigest --triplet $(VCPKG_TRIPLET)
	vcpkg install xdigest[$(VCPKG_FEATURES)] --enforce-port-checks \
		--overlay-ports=contrib/vcpkg --head --triplet $(VCPKG_TRIPLET)

# TODO: checksum
OPENSSL_VERSION = 3.6.0

openssl-$(OPENSSL_VERSION).zip:
	wget https://github.com/openssl/openssl/archive/refs/tags/openssl-$(OPENSSL_VERSION).zip

openssl: openssl-$(OPENSSL_VERSION).zip
	$(RMDIR) openssl openssl-openssl-$(OPENSSL_VERSION)
	unzip openssl-$(OPENSSL_VERSION).zip
	mv openssl-openssl-$(OPENSSL_VERSION) openssl

openssl-clean:
	$(RM) openssl-$(OPENSSL_VERSION)*.zip
	$(RMDIR) openssl
	$(RMDIR) openssl-$(OPENSSL_VERSION)

generate: openssl
	python3 generate.py

# release management
URL_STABLE = "^/xdigest/branches/$(VERSION_MAJOR).$(VERSION_MINOR).x"
URL_TAG = "^/xdigest/tags/$(VERSION_MAJOR).$(VERSION_MINOR).$(VERSION_PATCH)"

release-stable: clean test
	svn copy "^/xdigest/trunk" $(URL_STABLE)
	svn switch $(URL_STABLE)

switch-stable:
	svn switch $(URL_STABLE)

release-tag: switch-stable clean test
	svn copy $(URL_STABLE) $(URL_TAG)

# release management
DIST_TAG = tags/$(VERSION_MAJOR).$(VERSION_MINOR).$(VERSION_PATCH)
DIST_VERSION = $(subst tags/,,$(DIST_TAG))
KEY = 4734E1FDB2FAF97058D3141257E9B12AFBBE92B8

.PHONY: dist
dist: dist-test \
	dist/xdigest-$(DIST_VERSION).tar.gz \
	dist/xdigest-$(DIST_VERSION).tar.gz.sha256 \
	dist/xdigest-$(DIST_VERSION).tar.gz.sha512 \
	dist/xdigest-$(DIST_VERSION).zip \
	dist/xdigest-$(DIST_VERSION).zip.sha256 \
	dist/xdigest-$(DIST_VERSION).zip.sha512

.PHONY: dist-sign
dist-sign: dist \
	dist/xdigest-$(DIST_VERSION).tar.gz.asc \
	dist/xdigest-$(DIST_VERSION).zip.asc

.PHONY: dist-checkout
dist-checkout:
	$(RMDIR) dist/build
	svn checkout https://svn.rinrab.com/rinrab/xdigest/$(DIST_TAG) dist/build

dist-test: dist-checkout
	$(RMDIR) dist/test
	svn export dist/build dist/test
	make test -C dist/test
	$(RMDIR) dist/test

dist-publish: dist-test \
	dist/xdigest-$(DIST_VERSION).tar.gz \
	dist/xdigest-$(DIST_VERSION).tar.gz.sha256 \
	dist/xdigest-$(DIST_VERSION).tar.gz.sha512 \
	dist/xdigest-$(DIST_VERSION).tar.gz.asc \
	dist/xdigest-$(DIST_VERSION).zip \
	dist/xdigest-$(DIST_VERSION).zip.sha256 \
	dist/xdigest-$(DIST_VERSION).zip.sha512 \
	dist/xdigest-$(DIST_VERSION).zip.asc
	svnmucc -U https://svn.rinrab.com/files/xdigest/  \
		--message "upload $(DIST_VERSION) release files" \
		put dist/xdigest-$(DIST_VERSION).tar.gz xdigest-$(DIST_VERSION).tar.gz   \
		put dist/xdigest-$(DIST_VERSION).tar.gz.sha256 xdigest-$(DIST_VERSION).tar.gz.sha256 \
		put dist/xdigest-$(DIST_VERSION).tar.gz.sha512 xdigest-$(DIST_VERSION).tar.gz.sha512 \
		put dist/xdigest-$(DIST_VERSION).tar.gz.asc xdigest-$(DIST_VERSION).tar.gz.asc \
		put dist/xdigest-$(DIST_VERSION).zip xdigest-$(DIST_VERSION).zip   \
		put dist/xdigest-$(DIST_VERSION).zip.sha256 xdigest-$(DIST_VERSION).zip.sha256 \
		put dist/xdigest-$(DIST_VERSION).zip.sha512 xdigest-$(DIST_VERSION).zip.sha512 \
		put dist/xdigest-$(DIST_VERSION).zip.asc xdigest-$(DIST_VERSION).zip.asc

dist/xdigest-$(DIST_VERSION).tar.gz: dist-checkout
	$(RMDIR) dist/xdigest-$(DIST_VERSION)
	svn export dist/build dist/xdigest-$(DIST_VERSION)
	cd dist && tar -cf - xdigest-$(DIST_VERSION) | gzip -9 - > xdigest-$(DIST_VERSION).tar.gz 
	$(RMDIR) dist/xdigest-$(DIST_VERSION)

dist/xdigest-$(DIST_VERSION).zip: dist-checkout
	$(RMDIR) dist/xdigest-$(DIST_VERSION)
	svn export dist/build dist/xdigest-$(DIST_VERSION) --native-eol CRLF
	cd dist && zip -r -9 xdigest-$(DIST_VERSION).zip xdigest-$(DIST_VERSION)
	$(RMDIR) dist/xdigest-$(DIST_VERSION)

dist/xdigest-$(DIST_VERSION).tar.gz.asc: dist/xdigest-$(DIST_VERSION).tar.gz
	gpg --detach-sign --default-key $(KEY) --yes --armour --output $@ $^
dist/xdigest-$(DIST_VERSION).tar.gz.sha256: dist/xdigest-$(DIST_VERSION).tar.gz
	sha256sum $^ > $@
dist/xdigest-$(DIST_VERSION).tar.gz.sha512: dist/xdigest-$(DIST_VERSION).tar.gz
	sha512sum $^ > $@

dist/xdigest-$(DIST_VERSION).zip.asc: dist/xdigest-$(DIST_VERSION).zip
	gpg --detach-sign --default-key $(KEY) --yes --armour --output $@ $^
dist/xdigest-$(DIST_VERSION).zip.sha256: dist/xdigest-$(DIST_VERSION).zip
	sha256sum $^ > $@
dist/xdigest-$(DIST_VERSION).zip.sha512: dist/xdigest-$(DIST_VERSION).zip
	sha512sum $^ > $@

