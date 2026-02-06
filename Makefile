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
include build/rules.arch
include build/rules.unix

MAKE_FLAGS = \
	 "CC=$(CC)" \
	 "ASSEMBLER=$(ASSEMBLER)" \
	 "LINK_SHARED=$(LINK_SHARED)" \
	 "CONFIG=$(CONFIG)" \
	 "prefix=$(prefix)"

all:
	"$(MAKE)" -C xdigest $@ $(MAKE_FLAGS)

clean:
	"$(MAKE)" -C xdigest $@ $(MAKE_FLAGS)
	@find . -name '*.rej' | xargs $(RM)
	@find . -name '*.o' | xargs $(RM)
	@find . -name '*.so*' | xargs $(RM)
	$(RM) test_xdigest example

install:
	"$(MAKE)" -C xdigest $@ $(MAKE_FLAGS)

rebuild: clean all

xdigest/libxdigest.so: all

%.o: %.c
	mkdir -p $(@D)
	$(CC) $@ $^ -DXDIG -Ixdigest/include/xdigest

test_xdigest: tests/test_xdigest.o tests/sha_test.o xdigest/libxdigest.so
	$(MKDIR) $(@D)
	$(LINK_PROGRAM) $@ $^

example: tests/example.o xdigest/libxdigest.so
	$(MKDIR) $(@D)
	$(LINK_PROGRAM) $@ $^

test: test_xdigest
	export "LD_LIBRARY_PATH=$(CURDIR):$(LD_LIBRARY_PATH)"
	export "DYLD_LIBRARY_PATH=$(CURDIR):$(DYLD_LIBRARY_PATH)"
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
