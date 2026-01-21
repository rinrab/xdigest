/*
 * Copyright 1998-2023 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the Apache License 2.0 (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */

#include "internal/e_os.h"
/* ignored include 'crypto/cryptlib.h' */
#include "xhash/core.h"

#if     defined(__i386)   || defined(__i386__)   || defined(_M_IX86) || \
        defined(__x86_64) || defined(__x86_64__) || \
        defined(_M_AMD64) || defined(_M_X64)

#define xhash_IA32CAP_P_MAX_INDEXES 10

extern unsigned int xhash_ia32cap_P[xhash_IA32CAP_P_MAX_INDEXES];

# if defined(xhash_CPUID_OBJ)
#  define xhash_CPUID_SETUP
typedef uint64_t IA32CAP;

void xhash_cpuid_setup(void)
{
    static int trigger = 0;
    IA32CAP xhash_ia32_cpuid(unsigned int *);
    IA32CAP vec;

    if (trigger)
        return;

    trigger = 1;

    vec = xhash_ia32_cpuid(xhash_ia32cap_P);

    /*
     * |(1<<10) sets a reserved bit to signal that variable
     * was initialized already... This is to avoid interference
     * with cpuid snippets in ELF .init segment.
     */
    xhash_ia32cap_P[0] = (unsigned int)vec | (1 << 10);
    xhash_ia32cap_P[1] = (unsigned int)(vec >> 32);
}
# else
unsigned int xhash_ia32cap_P[xhash_IA32CAP_P_MAX_INDEXES];
# endif

#ifndef xhash_CPUID_OBJ
# ifndef xhash_CPUID_SETUP
void xhash_cpuid_setup(void)
{
}
# endif

/*
 * The rest are functions that are defined in the same assembler files as
 * the CPUID functionality.
 */

/*
 * The volatile is used to ensure that the compiler generates code that reads
 * all values from the array and doesn't try to optimize this away. The standard
 * doesn't actually require this behavior if the original data pointed to is
 * not volatile, but compilers do this in practice anyway.
 *
 * There are also assembler versions of this function.
 */
# undef xhash_memcmp
int xhash_memcmp(const void *in_a, const void *in_b, size_t len)
{
    size_t i;
    const volatile unsigned char *a = in_a;
    const volatile unsigned char *b = in_b;
    unsigned char x = 0;

    for (i = 0; i < len; i++)
        x |= a[i] ^ b[i];

    return x;
}

/*
 * For systems that don't provide an instruction counter register or equivalent.
 */
uint32_t xhash_rdtsc(void)
{
    return 0;
}

size_t xhash_instrument_bus(unsigned int *out, size_t cnt)
{
    return 0;
}

size_t xhash_instrument_bus2(unsigned int *out, size_t cnt, size_t max)
{
    return 0;
}

#endif /* ! xhash_CPUID_OBJ */

/* Public interface. */
void xhash_init()
{
    xhash_cpuid_setup();
}

#endif
