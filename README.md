# xdigest

Fast and lightweight hash library that serves OpenSSL assembly optimised hash
implementation as a standalone package.

## Features

- Fast; Uses assembly optimisation of digest functions to get best perfomance
  possible.
- Small and lightweight; Only includes code needed for digest computation
  itself. Nothing extra.
- Standalone; No dependencies are needed and it's easy to compile/install.
- Simple API; Just create a context and push data in it.

## Benchmarks

| algorithm | speed     |
|-----------|-----------|
| SHA1      | 1.50 GB/s |

## Getting started

This section describes how this library can be used in your projects.

If you plan to get it from a package manager, skip to the [Install library via a package manager](#install-library-via-a-package-manager)

If you plan to compile library from sources, you'll have to first get checkout
the source codes and compile it using either make or cmake. Both of these
options are explained bellow in further details. We've made it as simple and
straightforward as possible.

### Getting source code

The sources are officially published here at GitHub. But it's important to know
that the GitHub repository is actually a mirror of a Subversion repository
because svn is the GOAT.

```bash
git clone https://github.com/rinrab/xdigest.git
cd xdigest
```

### Install library via a package manager

Work in progress. I hope we'll be in their distrubutions at some point.

### Compile the library (CMake)

This project supports CMake build system for both, Unix and Windows platforms.
The basic process consists of configuration, compilation, (optional) testing,
and installation.

Please note that if you wish to compile xdigest on Windows platform with
assembly enabled, you would have to install NASM <https://www.nasm.us>. It's
important that its executable is either available in path or is passed via
CMAKE_ASM_NASM_COMPILER option.

Use the following command to configure project:

```bash
cmake -B out [...OPTIONS] [-G <generator>]
```

Available options are as follows:

- USE_ASM: Enables assembly implementations. Default is ON.
- CMAKE_INSTALL_PREFIX: Where to install project.
- ENABLE_TESTS: Whether to build the test-suite or not. Default is ON.
- BUILD_SHARED_LIBS: Builds into shared libraries (i.e. DLLs on Windows).
  Default is ON.

You could also change the output directory to build anywhere else by putting a
different path instead of `out`.

The command bellow will compile the project.

```bash
cmake --build out
```

The installation can be performed via a command like this one bellow:

```bash
sudo cmake --install out [--prefix <prefix>]
```

### Compile the library (Makefile)

You can also use `make` to compile the xdigest. However, this will only work on
Unix platforms. It might be beneficial to use this approach if you are on a
c-plus-plus-less platform/can't afford to install CMake. It would also be more
relevant for cross-compilation.

The following command will compile project:

```bash
make [...OPTIONS]
```

Supported options:

- `prefix`: where to install
- `NO_ASM`: disables assmebly
- `CONFIG`: disables assmebly

Now, we're ready to install it. Optionally, the install prefix could be changed
via `prefix` variable.

```
sudo make install [prefix=/usr/local] [OPTIONS]
```

Note: options, if any, have to passed to **all** steps.

### Use the library

This package exports headers <INCLUDEDIR>/xdigest/ and libxdigest.so library
object. Soon, we'll also add pkgconfig file.

Keep in mind that the library needs to be initialized first before using any
algorithms. It's important that initialization is done once per process. How
exactly it's done is responsibily of calling side. Since in our example we
don't really care about all that fancy multithreading stuff, we'll just invoke
it on startup.

For example, SHA related APIs are located in xdigest/sha.h. We could use to
create a context for processing SHA256 blocks.

```c
#include <assert.h>
#include <string.h>
#include <stdlib.h>

#include <xdigest.h>
#include <xdigest_sha2.h>

int main() {
    xdig_sha256_ctx_t *ctx = malloc(xdig_sha256_ctx_size());
    unsigned char digest1[XDIG_SHA256_DIGEST_LENGTH];
    unsigned char digest2[XDIG_SHA256_DIGEST_LENGTH];

    xdig_init();

    xdig_sha256_init(ctx);
    xdig_sha256_update(ctx, "abc", 3);
    xdig_sha256_update(ctx, "more things", sizeof("more things"));
    xdig_sha256_final(digest1, ctx);

    /* There is also a convenience wrapper. */
    xdig_sha256("abcmore things", sizeof("abcmore things"), digest2);

    /* Should be the same data. */
    assert(memcmp(digest1, digest2, sizeof(digest1)) == 0);

    return 0;
}
```

(this has not been tested yet)

## Design choices

### OpenSSL

This library originates its implementations from OpenSSL. The reason we do that
is because OpenSSL on its own has one of the best in terms of performance
implementations of hash algorithms. It has bullet-proven itself throughout many
years of being used everywhere.

### Code generation

The majority of source code is generated from OpenSSL source code. The
`generate.py` script in the source tree does this work. This guarantees
reproducebility and consistency of what we serve.

Generated files are also included in the source tree of the project for
simplity.

### Public headers

We provide lightweight and simple way to use OpenSSL's __implementation__.
However, nothing from OpenSSL should be exposed to public API. Everything in
include/xdigest/*.h should be re-written.

Checksum contexts are intended to be hidden from public API.

### Build process

It is indendent that the build process is as simple as possible. This is why
makefile is the choice. It doesn't do anything extra. Just compile+link. Also
fancy systems like CMake are sometimes anoying to deal with when assembly
integration is required.

### Initialization

To use capabilities of modern processors in order to compute digest checksums,
it has to be first initialized. Use `xdig_init()` function in `xdigest/xdigest.h`
header in order to do that. It is important that it's called once per process.
Otherwise, something might missbehave.

## TODO checklist

- [ ] Platforms
    - [x] Linux
    - [ ] Windows
    - [x] 64 bit
    - [x] 32 bit
    - [x] ARM
- [x] SHA1
- [x] SHA2
- [ ] SHA3
- [x] MD2
- [x] MD4
- [x] MD5
- [x] cpuid initalization
- [x] version identification
- [ ] Package distributions
  - [ ] Debian
  - [ ] Arch Linux
  - [ ] vcpkg
- [ ] Lower priority package distributions
  - [ ] RPM
  - [ ] brew
  - [ ] Mac Ports

## Milestones

(the list is not really completed)

### Alpha

Fully-functioning library. Supports basic hash algorithms. Can compute digest
blazingly fast using assembly optimised implementation.

Only Unix support is planed for the alpha version. It doesn't mean Windows is
not going to be supported in general, but focus will be concentrated on Unix
specifically.

`(We are here. Move this marker as we go.)`

Build process works perfectly, including cross-compiling, installation, and
out-of-tree build.

APIs are not yet stable. No backward compatibility is guaranteed.

Decide about the name of the project. 'xhash' sounds good but 1) a library with
this name (similar) already exists and 2) it's actually digest, not hash or
checksum. It could be called 'xdigest', which is also a good one, but too long,
especially when considering symbol names (Like xdigest_sha1_ctx_init would be
overkill. It's a bit nonsense that 'digest' and 'sha1' are in the same name).
I'm also considering something originating from 'hyper-sonic' hashes or
something with 'lightweight' but couldn't think of any good ideas to combine
those.

### Beta

Focus on stabilisation and moving towards being included into various package
destributions. It will also get its 1.0 version number.

A nice place in Apache Subversion's codebase exists where this library will be
extreamly useful. We'll be suggested as an option for its checksum backend
library.

Now, it's also time to support Windows platform. Most probably, we should be
using NMake build-system for that (If it can be called as build system. I
believe makefiles are not really a "build system").

Some fancy improvents in build process could also be done. These include, for
example, supporting no-libc configuration, building only some specific
algorithm, or making a fully-static library.

### Gamma

Everything works just fine. People start using this library as a mainstream
choise as fast digest is required.
