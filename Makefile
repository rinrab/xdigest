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
