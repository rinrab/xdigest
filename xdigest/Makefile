# NO_ASM - disables assembly
# CC - c compiler
# ASSEMBLER - asm compiler
# ASMFLAGS - things to pass to the assembler
# prefix - where to install stuff

ifdef NO_ASM
    DEFS += -DNO_ASM
else
    DEFS += -Dxdig_CPUID_OBJ
    DEFS += -DMD5_ASM
    DEFS += -DSHA1_ASM -DSHA256_ASM -DSHA512_ASM
endif

include ../build/rules.unix
include Makefile.inc

INCS += -Iinclude -Iinclude/xdigest -I. -Icore

objects = $(c_objects)
ifndef NO_ASM
    objects += $(asm_objects)
endif

include ../build/Version.inc

sofiles = \
	libxdigest.so.$(VERSION) \
	libxdigest.so.$(SONAME) \
	libxdigest.so

all: $(sofiles)

install: install-shared install-headers

clean:
	$(RM) $(sofiles)
	$(RM) $(objects)

libxdigest.so.$(VERSION): $(objects)
	$(MKDIR) $(@D)
	$(LINK_SHARED) $@ $^

libxdigest.so.$(SONAME): libxdigest.so.$(VERSION)
	$(LN) $^ $@

libxdigest.so: libxdigest.so.$(SONAME)
	$(LN) $^ $@

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
	install -m 644 include/xdigest/*.h $(prefix)/include/xdigest
