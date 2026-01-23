# xhash

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

### Getting source code

Currenly, the only way to install the library is to compile it using 'make'
from sources. It's simple anyway.

First, get the sources. The sources are officially published here at GitHub.
But it's important to know that the GitHub repository is actually a mirror of a
Subversion repository because svn is the GOAT.

```bash
git clone https://github.com/rinrab/xhash.git
cd xhash
```

### Install library via a package manager

Work in progress. I hope we'll be in their distrubutions at some point.

### Compile the library (Unix)

Let's compile it. It shouldn't take more than 0.73 seconds. However, it might
take 0.86 seconds in the worst case scenario. The process itself is also as
straightforward as it sounds.

```bash
make
```

If you really want to, assembly-backed implementation can be disabled via
NO_ASM option.

```bash
make [NO_ASM=1]
```

Now, we're ready to install it. Optionally, the install prefix could be changed
via PREFIX variable.

```
sudo make install [PREFIX=/usr/local]
```

To cross compile, pass any of the following value to the ARCH variable while
performing **all** operations.

### Compile the library (Win32)

The best talents of this world are currently working hard to get this done.

### Use the library

This package exports headers <INCLUDEDIR>/xhash/ and libxhash.so library
object. Soon, we'll also add pkgconfig file.

Keep in mind that the library needs to be initialized first before using any
algorithms. It's important that initialization is done once per process. How
exactly it's done is responsibily of calling side. Since in our example we
don't really care about all that fancy multithreading stuff, we'll just invoke
it on startup.

For example, SHA related APIs are located in xhash/sha.h. We could use to
create a context for processing SHA256 blocks.

```c
#include <assert.h>
#include <string.h>

#include <xhash/xhash.h>
#include <xhash/sha.h>

int main() {
    xhash_sha256_ctx_t ctx = { 0 };
    char digest1[XHASH_SHA256_DIGEST_LENGTH];
    char digest2[XHASH_SHA256_DIGEST_LENGTH];

    xhash_init();

    xhash_sha256_init(&ctx);
    xhash_sha256_update(&ctx, "abc", 3);
    xhash_sha256_update(&ctx, "more things", sizeof("more things"));
    xhash_sha256_final(&digest1, &ctx);

    /* There is also a convenience wrapper. */
    xhash_sha256("abcmore things", sizeof("abcmore things"), &digest2);

    /* Should be the same data. */
    assert(memcmp(digest1, digest2, sizeof(digest1)) == 0);

    return 0;
}
```

(this has not been tested yet)

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
