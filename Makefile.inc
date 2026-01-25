# CONFIG -- which assemblies to use
#
# OBJEXT -- extensions of object files as they are compiled on current system.
# will be just appended to file names. examples: ".o", ".obj"

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

