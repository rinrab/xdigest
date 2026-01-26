# NO_ASM - disables assembly
# CC - c compiler
# ASSEMBLER - asm compiler
# CFLAGS - things to pass to the compiler
# ASMFLAGS - things to pass to the assembler
# prefix - where to install stuff

ifdef NO_ASM
    DEFS += -DNO_ASM
else
    DEFS += -Dxdig_CPUID_OBJ
    DEFS += -DMD5_ASM
    DEFS += -DSHA1_ASM -DSHA256_ASM -DSHA512_ASM
endif

include Makefile.inc
include ../rules.unix

INCS += -Iinclude

objects = $(c_objects)
ifndef NO_ASM
    objects += $(asm_objects)
endif

include ../Version.inc

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
	$(LINK_SHARED) -o $@ $^

libxdigest.so.$(SONAME): libxdigest.so.$(VERSION)
	$(LN) $^ $@

libxdigest.so: libxdigest.so.$(SONAME)
	$(LN) $^ $@

$(asm_objects): %.o: %.S
	mkdir -p $(@D) && $(ASSEMBLER) -c $^ -o $@ $(ASMFLAGS)

$(c_objects): %.o: %.c
	mkdir -p $(@D) && $(CC) -c $^ -o $@ $(CFLAGS) $(DEFS) $(INCS)

install-shared: $(sofiles)
	install -d $(DESTDIR)$(prefix)/lib/
	install -m 644 $(sofiles) $(DESTDIR)$(prefix)/lib/

install-headers:
	install -d $(DESTDIR)$(prefix)/include/
	install -d $(DESTDIR)$(prefix)/include/xdigest
	install -m 644 include/xdigest/*.h $(DESTDIR)$(prefix)/include/xdigest
