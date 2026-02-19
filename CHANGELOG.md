# Changelog 

All notable changes should be documented here.

## 0.5.0

### Tar-ball Distributions

Starting from 0.5.0, we distribute signed tar-balls with source code for both,
Unix and Windows. It's recommended for Unix consumers to use .tar.gz and
Windows users should use .zip archives.

### Build Improvements

Compilation for Risc-V and PowerPC architectures and non-canonical ARM versions
has been fixed. We now provide assembly implementation for Risc-V platform.
However, those platforms have not been tested enough and might not work as
expected.

PowerPC and ARM v4-v7 assemblies are still to be done.

CMake's USE_ASM option can now automatically detect whether assembly is
supported or not. If target implementation does not support it, it will will be
disabled.

### Rust Bindings

An experiemntal version of bindings for rust was added to the project. Its
sources can be found in contrib/rust/ directory of the source tree. Those
include a source code distribution for the package manager and a system package
that compiles those sources.

There are plenty of things to be done to make it production ready yet. Those
include the following:

- High-level wrapper.
- Use of system-installed library if available.
- Perform some testing.

## 0.4.0

This release focuses on tidying up the source code. Nothing really meaningfull
should change for you as a user. 

- Cleanup symbols. This includes removing some functions which were never used
  but yoinked from OpenSSL and renaming the rest to begin with `xdig` prefix.
  Also cleanup similar nonsence from the headers.

- Add copyright headers to the source files.

Some issue have been adressed in my pull-request to the vcpkg registry
<https://github.com/microsoft/vcpkg/pull/49708>.

## 0.3.0

In this version, the ABI of the library is broken. We didn't promise it at this
stage anyways. This release is focused on improving overall experience of
consuming the library.

### API improvements

The public API has been canonicalised. Now each function follows the following
convention:

```c
<type> xdig_<namespace>_<verb>(<self> <output parameter...> <options...>);
```

Here self is a structure pointer to the state of itself. It's like obj.method()
in object oriented languages. Ouput paramters must be before every other option
but after self. The rest are considered options and should be at the end of the
declaration.

In the follwing methods self has been moved to the first argument (r6455):

```
-xdig_sha1_ctx_final(unsigned char *digest, xdig_sha1_ctx_t *ctx)
+xdig_sha1_ctx_final(xdig_sha1_ctx_t *ctx, unsigned char *digest)

-xdig_sha224_ctx_final(unsigned char *digest, xdig_sha224_ctx_t *ctx)
+xdig_sha224_ctx_final(xdig_sha224_ctx_t *ctx, unsigned char *digest)

-xdig_sha256_ctx_final(unsigned char *digest, xdig_sha256_ctx_t *ctx)
+xdig_sha256_ctx_final(xdig_sha256_ctx_t *ctx, unsigned char *digest)

-xdig_sha384_ctx_final(unsigned char *digest, xdig_sha384_ctx_t *ctx)
+xdig_sha384_ctx_final(xdig_sha384_ctx_t *ctx, unsigned char *digest)

-xdig_sha512_ctx_final(unsigned char *digest, xdig_sha512_ctx_t *ctx)
+xdig_sha512_ctx_final(xdig_sha512_ctx_t *ctx, unsigned char *digest)

-xdig_md2_ctx_final(unsigned char *digest, xdig_md2_ctx_t *ctx)
+xdig_md2_ctx_final(xdig_md2_ctx_t *ctx, unsigned char *digest)

-xdig_md4_ctx_final(unsigned char *digest, xdig_md4_ctx_t *ctx)
+xdig_md4_ctx_final(xdig_md4_ctx_t *ctx, unsigned char *digest)

-xdig_md5_ctx_final(unsigned char *digest, xdig_md5_ctx_t *ctx)
+xdig_md5_ctx_final(xdig_md5_ctx_t *ctx, unsigned char *digest)
```

In the following functions output parameters were moved to the begining of the
function (r6477):

```
-unsigned char *xdig_md2(const void *data, size_t len, unsigned char *digest);
+unsigned char *xdig_md2(unsigned char *digest, const void *data, size_t len);

-unsigned char *xdig_md4(const void *data, size_t len, unsigned char *digest);
+unsigned char *xdig_md4(unsigned char *digest, const void *data, size_t len);

-unsigned char *xdig_md5(const void *data, size_t len, unsigned char *digest);
+unsigned char *xdig_md5(unsigned char *digest, const void *data, size_t len);

-unsigned char *xdig_sha1(const void *data, size_t len, unsigned char *digest);
+unsigned char *xdig_sha1(unsigned char *digest, const void *data, size_t len);

-unsigned char *xdig_sha224(const void *data, size_t len, unsigned char *digest);
+unsigned char *xdig_sha224(unsigned char *digest, const void *data, size_t len);

-unsigned char *xdig_sha256(const void *data, size_t len, unsigned char *digest);
+unsigned char *xdig_sha256(unsigned char *digest, const void *data, size_t len);

-unsigned char *xdig_sha384(const void *data, size_t len, unsigned char *digest);
+unsigned char *xdig_sha384(unsigned char *digest, const void *data, size_t len);

-unsigned char *xdig_sha512(const void *data, size_t len, unsigned char *digest);
+unsigned char *xdig_sha512(unsigned char *digest, const void *data, size_t len);
```

Also some cosmetic work has been made in the header and source files towards
naming argument in a predictable and meaningfull way.

### Codebase changes

In this release, a decision was made to stop generation source files of C
implementation (and some wrappers) via a script. Instead, now we maintain them
as a fork (r6454). Below is the motivation for that from the log message:

```
What this means? Basically, we no longer want to "derive" openssl but to fully
"fork" it. It's anoying to maintain those patches as their amount grows to the
moon.

There are plenty of changes we could make in the way openssl is coded. Because
this is currently trash.
```

This way it would be much more convenient to add new features and improve
internal source code. It makes refactoring much easier. Which is one of the
points and aims of this project in general. We produce simple for use library
that doesn't do anything crazy or that user didn't ask about.

We are now including public headers into implementations of each digest
algorithm. Before we used to dublicate them. We had the public header which was
hand-written from blank and an internal one which was generated directly from
OpenSSL sources. The symbols were defined twice in both of these headers. While
the public header only provided struct-typdef for the contexts, internally they
were represented in a full structure. By now, the definitions are droped from
the internal headers, and included directly from the public one. This allowed
to prevent duplication and find a few missmatches between the declarations and
the implementations.

Some code was factored out from sha256.c and sha512.c into separate
sha256_local.h and sha512_local.h header files. This mostly included low-level
block updates. This is a common practice in OpenSSL itself, but wasn't applied
to these two files.

Single block transform functions were droped from the code base (r6468). We
weren't exporting them apart of the public API anyway. No-one should use them.
Please refer to full updates and specify length explicty. If it turned out that
exactly one block was passed, we're lucky. If not, it still works in a pretty
much the same way. Users should not expect any performance throw-aways by not
aligning block perfectly. On the other hand, it's much more convenient for us
to provide a single method for everything. Let's keep things simple.

### Minor fixes and improvements

- Added SOVERSION to cmake build (r6470). Now it produces the following shared
objects:

```
libxdigest.so
libxdigest.so.0
libxdigest.so.0.3.0
```

- The ARM GitHub action workflow is dropped because it was never run (r6480).

## 0.2.0

The 0.2.x release is more like a follow-up to 0.1.x. It mostly carries fixes to
the build system and improves overall user experience. The following changes
have been made to the library and contribution infrastructure:

- Fixed build on Windows/ARM64 by adjusting winapi call used.
- Support MacOS platform.
- Add and install CMake config.
- Add CMake module to help users link the library.
- Improved/cleaned-up vcpkg and archlinux ports.

Also at this period of time, a port for Arch User Repository (AUR) was
published for the first time <https://aur.archlinux.org/packages/xdigest>.

## 0.1.0

This is an initial release of the library. Please note that we don't provide
for backward compatibility as we're in 0.x stage.

This version brings a fully-functioning library that already supports assembly
backend.

There are two build-systems supported; make and cmake. More information could
be found in the top-level readme file of the project.

The following set of algorithms are currently supported:

- SHA1 (with asm)
- SHA2 (224/256/384/512), with asm
- MD5 (with asm)
- MD4 (without asm)
- MD2 (without asm)

1.0.x is planned soon after 0.1.x.
