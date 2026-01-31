# Changelog

All notable changes should be documented here.

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
