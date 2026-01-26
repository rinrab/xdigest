out = out

include Version.inc
include rules.unix

MAKE_FLAGS = \
	 "CC=$(CC)"  \
	 "ASSEMBLER=$(ASSEMBLER)" \
	 "CONFIG=$(CONFIG)" \
	 "CFLAGS=$(CFLAGS)" \
	 "ASMFLAGS=$(CFLAGS)" \
	 "prefix=$(prefix)"

all:
	$(MAKE) -C xdigest $@ $(MAKE_FLAGS)

clean:
	$(MAKE) -C xdigest $@ $(MAKE_FLAGS)
	@find . -name '*.rej' | xargs $(RM)

install:
	$(MAKE) -C xdigest $@ $(MAKE_FLAGS)

rebuild: clean all

xdigest/libxdigest.so: all

test_xdigest: tests/test_xdigest.c tests/sha_test.c xdigest/libxdigest.so
	$(MKDIR) $(@D) && $(CC) $^ -o $@ $(CFLAGS) -DXDIG -Ixdigest/include

example: tests/example.c xdigest/libxdigest.so
	$(MKDIR) $(@D) && $(CC) $^ -o $@ $(CFLAGS) -DXDIG -Ixdigest/include

test: test_xdigest
	export "LD_LIBRARY_PATH=$(CURDIR):$(LD_LIBRARY_PATH)" && ./test_xdigest

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
