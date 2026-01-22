# xhash

Fast and lightweight hash library that serves OpenSSL assembly optimised hash
implementation as a standalone package.

## Features

Fast, small, and standalone.

## Benchmarks

| algorithm | speed     |
|-----------|-----------|
| SHA1      | 1.50 GB/s |

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
