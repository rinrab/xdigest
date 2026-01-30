/*
 * This is one of the most beautyfull ways to detect architecture. We compile
 * this snippet in our cmake script. and scrape its error output, matching the
 * correct configuration via dedicated keyword..
 *
 * Native cmake tools sometimes behaves upredictebly and are anoying to deal
 * with. This is IMO the most relayable way to tackle this issue. This way we
 * integrate directly to the compile. Because sometimes not the options define
 * architecture, but its built into the program itself. Especially on Unix-ish
 * platforms.
 *
 * We might also need a separte detector which checks whether the compiler
 * works in general. Because this script takes error output entirely. And
 * always fails no matter what.
 *
 * We cannot simply run it because then we would lose ability to cross-compile.
 * It's not usually possible to run program compiled for different architecture
 * on host one.
 */

#ifdef __linux__

#if defined(__x86_64__) || defined(_M_X64)
# error "DETECTED: linux64"
#elif defined(i386) || defined(__i386__) || defined(__i386) || defined(_M_IX86)
# error "DETECTED: linux32"
#elif defined(__aarch64__) || defined(_M_ARM64)
# error "DETECTED: linuxaarch64"
#endif

#elif defined(__APPLE__)
#if defined(__x86_64__) || defined(_M_X64)
# error "DETECTED: macosx64"
#elif defined(i386) || defined(__i386__) || defined(__i386) || defined(_M_IX86)
# error "DETECTED: macosx32"
#elif defined(__aarch64__) || defined(_M_ARM64)
# error "DETECTED: macosxaarch64"
#endif

#elif WIN32
#if defined(__x86_64__) || defined(_M_X64)
# error "DETECTED: win64"
#elif defined(i386) || defined(__i386__) || defined(__i386) || defined(_M_IX86)
# error "DETECTED: win32"
#elif defined(__aarch64__) || defined(_M_ARM64)
# error "DETECTED: winaarch64"
#endif

#else
# error "DETECTED: unsupported"
#endif
