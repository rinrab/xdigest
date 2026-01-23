# CONFIG -- which assemblies to use
#
# OBJEXT -- extensions of object files as they are compiled on current system.
# will be just appended to file names. examples: ".o", ".obj"

c_objects = \
    src/core/cpuid$(OBJEXT) \
    src/core/armcap$(OBJEXT) \
    src/core/version$(OBJEXT) \
    src/core/mem_clr$(OBJEXT) \
    src/sha/sha1$(OBJEXT) \
    src/sha/sha256$(OBJEXT) \
    src/sha/sha512$(OBJEXT) \
    src/md5/md5_dgst$(OBJEXT) \
    src/md5/md5_one$(OBJEXT) \
    src/md4/md4_dgst$(OBJEXT) \
    src/md4/md4_one$(OBJEXT) \
    src/md2/md2_dgst$(OBJEXT) \
    src/md2/md2_one$(OBJEXT)

asm_objects = \
    src/core/asm/cpuid-$(CONFIG)$(OBJEXT) \
    src/sha/asm/sha256-$(CONFIG)$(OBJEXT) \
    src/sha/asm/sha512-$(CONFIG)$(OBJEXT) \
    src/sha/asm/sha1-$(CONFIG)$(OBJEXT) \
    src/md5/asm/md5-$(CONFIG)$(OBJEXT)

