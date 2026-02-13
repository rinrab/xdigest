# xdigest

Extremely fast digest algorithm implementations packaged into a lightweight library.

## Features

- Fast; Uses assembly optimisation of digest functions to get best perfomance
  possible.
- Small and lightweight; Only includes code needed for digest computation
  itself. Nothing extra.
- Standalone; No dependencies are needed and it's easy to compile/install.
- Simple API; Just create a context and push data in it.

## Benchmarks

To get more accurate benchmarks on your machine, you can run the test suite.
It's is usualy done via `make test` command. It will dump benchmarks of feeding
random data into hash context by 1 MB blocks and dump results per each
supported algorithm.

Below, is a table of test results ran locally on my power-plant laptop (with
Linux).

| algorithm | with assembly | without assembly (NO_ASM=1) |
|---|---|---|
|   sha1 | 1.91 GB/s | 0.85 GB/s |
| sha224 | 1.71 GB/s | 0.30 GB/s |
| sha256 | 1.71 GB/s | 0.30 GB/s |
| sha384 | 0.75 GB/s | 0.50 GB/s |
| sha512 | 0.75 GB/s | 0.50 GB/s |
|    md5 | 0.78 GB/s | 0.70 GB/s |
|    md4 | 1.25 GB/s | 1.26 GB/s |
|    md2 | 0.01 GB/s | 0.01 GB/s |

These results could be not accurate enough because I remember getting much
higher benchmarks before I reconfigured something (I don't even know what
exactly have I changed) in my system in relation to powersaving modes.

I also made an interesting observation that I was getting around 1.50 GB/s
(SHA1) in WSL environment and even lower number on Windows host. The peak I
could get on Linux was 2.50 GB/s but I will probably have to figure out how TLP
works to get those measurements again. Average Linux desktop experience...

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

#### Arch Linux

```
yay -S xdigest
```

#### vcpkg

```
vcpkg install xdigest
```

### Compile the library (CMake)

This project supports CMake build system for both, Unix and Windows platforms.
The basic process consists of configuration, compilation, (optional) testing,
and installation.

Please note that if you wish to compile xdigest on Windows platform with
assembly enabled, you would have to install NASM <https://www.nasm.us>. It's
important that its executable is either available in path or is passed via
`CMAKE_ASM_NASM_COMPILER` option.

On Windows, it is also mandatory that all commands are executed from the Visual
Studio development command prompt. This will ensure that everything including
cmake and the compiler itself is available in PATH.

Use the following command to configure project:

```bash
cmake -B out [...OPTIONS] [-G <generator>]
```

Available options are as follows:

- `USE_ASM`: Enables assembly implementations. Default is ON.
- `CMAKE_INSTALL_PREFIX`: Where to install project.
- `ENABLE_TESTS`: Whether to build the test-suite or not. Default is ON.
- `BUILD_SHARED_LIBS`: Builds into shared libraries (i.e. DLLs on Windows).
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

This package exports headers `<INCLUDEDIR>/xdigest/` and libxdigest.so library
object. Also pkgconfig files are exported as `xdigest.pc`.

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

    xdig_sha256_ctx_init(ctx);
    xdig_sha256_ctx_update(ctx, "abc", 3);
    xdig_sha256_ctx_update(ctx, "more things", sizeof("more things"));
    xdig_sha256_ctx_final(ctx, digest1);

    /* There is also a convenience wrapper. */
    xdig_sha256("abcmore things", sizeof("abcmore things"), digest2);

    /* Should be the same data. */
    assert(memcmp(digest1, digest2, sizeof(digest1)) == 0);

    return 0;
}
```

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

It is intendent that the build process is as simple as possible. This is why
makefile was the original choice. It doesn't do anything extra. Just
compile+link. CMake was added later on. It should not do anything crazy as
well.

### Initialization

To use capabilities of modern processors in order to compute digest checksums,
it has to be first initialized. Use `xdig_init()` function in `xdigest/xdigest.h`
header in order to do that. It is important that it's called once per process.
Otherwise, something might missbehave.

## Copyright

This product is published under Apache-2.0 license. Please refer to LICENSE and
NOTICE for more information. Many source files have been derived from OpenSSL.
Those files must retain copyright header in each source file.

Every file must begin with a comment with Apache-2.0 apendix and copyright of
original source, if any. The first line should include a filename and short
description. Multiple licenses should be separated with a bunch of '=' acting
as a separator (and enclose their contents). Feel free to yank to from
elsewhere in the codebase.

Everyone who contributed to a source file should include their name into
license header. This is more like a recommendation, not a requirement. After a
change in a source file, year should be updated. Don't bother doing those
modification if a change was obvious or minor. Use your own judgement and
preferences.
