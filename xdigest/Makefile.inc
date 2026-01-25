# CONFIG -- which assemblies to use
#
# OBJEXT -- extensions of object files as they are compiled on current system.
# will be just appended to file names. examples: ".o", ".obj"

c_objects = \
    core/cpuid$(OBJEXT) \
    core/armcap$(OBJEXT) \
    core/version$(OBJEXT) \
    core/mem_clr$(OBJEXT) \
    sha/sha1$(OBJEXT) \
    sha/sha256$(OBJEXT) \
    sha/sha512$(OBJEXT) \
    md5/md5_dgst$(OBJEXT) \
    md5/md5_one$(OBJEXT) \
    md4/md4_dgst$(OBJEXT) \
    md4/md4_one$(OBJEXT) \
    md2/md2_dgst$(OBJEXT) \
    md2/md2_one$(OBJEXT)

asm_objects = \
    core/asm/cpuid-$(CONFIG)$(OBJEXT) \
    sha/asm/sha256-$(CONFIG)$(OBJEXT) \
    sha/asm/sha512-$(CONFIG)$(OBJEXT) \
    sha/asm/sha1-$(CONFIG)$(OBJEXT) \
    md5/asm/md5-$(CONFIG)$(OBJEXT)

