/*
 * core/cpuid.c: generic CPU initialization
 *
 * ====================================================================
 * Copyright 1998-2023 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the Apache License 2.0 (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 * ====================================================================
 * Copyright 2026, Timofei Zhakov
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * ====================================================================
 */

#if     defined(__i386)   || defined(__i386__)   || defined(_M_IX86) || \
        defined(__x86_64) || defined(__x86_64__) || \
        defined(_M_AMD64) || defined(_M_X64)

#include "internal/e_os2.h"
/* ignored include 'crypto/cryptlib.h' */
#include "xdigest/xdigest.h"
#include <stddef.h>

#define xdig_IA32CAP_P_MAX_INDEXES 10

extern unsigned int xdig_ia32cap_P[xdig_IA32CAP_P_MAX_INDEXES];

# if defined(xdig_CPUID_OBJ)
#  define xdig_CPUID_SETUP
typedef uint64_t IA32CAP;

void xdig_cpuid_setup(void)
{
    static int trigger = 0;
    IA32CAP xdig_ia32_cpuid(unsigned int *);
    IA32CAP vec;

    if (trigger)
        return;

    trigger = 1;

    vec = xdig_ia32_cpuid(xdig_ia32cap_P);

    /*
     * |(1<<10) sets a reserved bit to signal that variable
     * was initialized already... This is to avoid interference
     * with cpuid snippets in ELF .init segment.
     */
    xdig_ia32cap_P[0] = (unsigned int)vec | (1 << 10);
    xdig_ia32cap_P[1] = (unsigned int)(vec >> 32);
}
# else
unsigned int xdig_ia32cap_P[xdig_IA32CAP_P_MAX_INDEXES];
# endif

#ifndef xdig_CPUID_OBJ
# ifndef xdig_CPUID_SETUP
void xdig_cpuid_setup(void)
{
}
# endif

/*
 * The rest are functions that are defined in the same assembler files as
 * the CPUID functionality.
 */

/*
 * For systems that don't provide an instruction counter register or equivalent.
 */
uint32_t xdig_rdtsc(void)
{
    return 0;
}

size_t xdig_instrument_bus(unsigned int *out, size_t cnt)
{
    return 0;
}

size_t xdig_instrument_bus2(unsigned int *out, size_t cnt, size_t max)
{
    return 0;
}

#endif /* ! xdig_CPUID_OBJ */

/* Public interface. */
void xdig_init()
{
    xdig_cpuid_setup();
}

#endif
