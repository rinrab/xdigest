/*
 * core/ppccap.c: CPU initialization for PPC processors
 *
 * ====================================================================
 * Copyright 2009-2025 The OpenSSL Project Authors. All Rights Reserved.
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

#if \
  defined(__powerpc) || defined(__powerpc__) || defined(__powerpc64__) || \
  defined(__POWERPC__) || defined(__ppc__) || defined(__PPC__) || \
  defined(__PPC64__) || defined(__ppc64__) || defined(_ARCH_PPC64)

#ifdef xdig_CPUID_OBJ 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>
#include <signal.h>
#include <unistd.h>
#if defined(__linux) || defined(_AIX)
# include <sys/utsname.h>
#endif
#if defined(_AIX53)     /* defined even on post-5.3 */
# include <sys/systemcfg.h>
# if !defined(__power_set)
#  define __power_set(a) (_system_configuration.implementation & (a))
# endif
#endif
#if defined(__APPLE__) && defined(__MACH__)
# include <sys/types.h>
# include <sys/sysctl.h>
#endif
#include <openssl/crypto.h>
#include "internal/cryptlib.h"
#include "crypto/ppc_arch.h"

unsigned int xdig_ppccap_P = 0;

static sigset_t all_masked;

static sigjmp_buf ill_jmp;
static void ill_handler(int sig)
{
    siglongjmp(ill_jmp, sig);
}

void xdig_fpu_probe(void);
void xdig_ppc64_probe(void);
void xdig_altivec_probe(void);
void xdig_crypto207_probe(void);
void xdig_madd300_probe(void);
void xdig_brd31_probe(void);

long xdig_rdtsc_mftb(void);
long xdig_rdtsc_mfspr268(void);

uint32_t xdig_rdtsc(void)
{
    if (xdig_ppccap_P & PPC_MFTB)
        return xdig_rdtsc_mftb();
    else if (xdig_ppccap_P & PPC_MFSPR268)
        return xdig_rdtsc_mfspr268();
    else
        return 0;
}

size_t xdig_instrument_bus_mftb(unsigned int *, size_t);
size_t xdig_instrument_bus_mfspr268(unsigned int *, size_t);

size_t xdig_instrument_bus(unsigned int *out, size_t cnt)
{
    if (xdig_ppccap_P & PPC_MFTB)
        return xdig_instrument_bus_mftb(out, cnt);
    else if (xdig_ppccap_P & PPC_MFSPR268)
        return xdig_instrument_bus_mfspr268(out, cnt);
    else
        return 0;
}

size_t xdig_instrument_bus2_mftb(unsigned int *, size_t, size_t);
size_t xdig_instrument_bus2_mfspr268(unsigned int *, size_t, size_t);

size_t xdig_instrument_bus2(unsigned int *out, size_t cnt, size_t max)
{
    if (xdig_ppccap_P & PPC_MFTB)
        return xdig_instrument_bus2_mftb(out, cnt, max);
    else if (xdig_ppccap_P & PPC_MFSPR268)
        return xdig_instrument_bus2_mfspr268(out, cnt, max);
    else
        return 0;
}

#if defined(__GLIBC__) && defined(__GLIBC_PREREQ)
# if __GLIBC_PREREQ(2, 16)
#  include <sys/auxv.h>
#  define OSSL_IMPLEMENT_GETAUXVAL
# elif defined(__ANDROID_API__)
/* see https://developer.android.google.cn/ndk/guides/cpu-features */
#  if __ANDROID_API__ >= 18
#   include <sys/auxv.h>
#   define OSSL_IMPLEMENT_GETAUXVAL
#  endif
# endif
#endif

#if defined(__FreeBSD__) || defined(__OpenBSD__)
# include <sys/param.h>
# if (defined(__FreeBSD__) && __FreeBSD_version >= 1200000) || \
    (defined(__OpenBSD__) && OpenBSD >= 202409)
#  include <sys/auxv.h>
#  define OSSL_IMPLEMENT_GETAUXVAL

static unsigned long getauxval(unsigned long key)
{
  unsigned long val = 0ul;

  if (elf_aux_info((int)key, &val, sizeof(val)) != 0)
    return 0ul;

  return val;
}
# endif
#endif

/* I wish <sys/auxv.h> was universally available */
#ifndef AT_HWCAP
# define AT_HWCAP               16      /* AT_HWCAP */
#endif
#define HWCAP_PPC64             (1U << 30)
#define HWCAP_ALTIVEC           (1U << 28)
#define HWCAP_FPU               (1U << 27)
#define HWCAP_POWER6_EXT        (1U << 9)
#define HWCAP_VSX               (1U << 7)

#ifndef AT_HWCAP2
# define AT_HWCAP2              26      /* AT_HWCAP2 */
#endif
#define HWCAP_VEC_CRYPTO        (1U << 25)
#define HWCAP_ARCH_3_00         (1U << 23)
#define HWCAP_ARCH_3_1          (1U << 18)

void xdig_cpuid_setup(void)
{
    char *e;
    struct sigaction ill_oact, ill_act;
    sigset_t oset;
    static int trigger = 0;

    if (trigger)
        return;
    trigger = 1;

    xdig_ppccap_P = 0;

#if defined(_AIX)
    xdig_ppccap_P |= PPC_FPU;

    if (sizeof(size_t) == 4) {
        struct utsname uts;
# if defined(_SC_AIX_KERNEL_BITMODE)
        if (sysconf(_SC_AIX_KERNEL_BITMODE) != 64)
            return;
# endif
        if (uname(&uts) != 0 || atoi(uts.version) < 6)
            return;
    }

# if defined(__power_set)
    /*
     * Value used in __power_set is a single-bit 1<<n one denoting
     * specific processor class. Incidentally 0xffffffff<<n can be
     * used to denote specific processor and its successors.
     */
    if (sizeof(size_t) == 4) {
        /* In 32-bit case PPC_FPU64 is always fastest [if option] */
        if (__power_set(0xffffffffU<<13))       /* POWER5 and later */
            xdig_ppccap_P |= PPC_FPU64;
    } else {
        /* In 64-bit case PPC_FPU64 is fastest only on POWER6 */
        if (__power_set(0x1U<<14))              /* POWER6 */
            xdig_ppccap_P |= PPC_FPU64;
    }

    if (__power_set(0xffffffffU<<14))           /* POWER6 and later */
        xdig_ppccap_P |= PPC_ALTIVEC;

    if (__power_set(0xffffffffU<<16))           /* POWER8 and later */
        xdig_ppccap_P |= PPC_CRYPTO207;

    if (__power_set(0xffffffffU<<17))           /* POWER9 and later */
        xdig_ppccap_P |= PPC_MADD300;

    if (__power_set(0xffffffffU<<18))           /* POWER10 and later */
        xdig_ppccap_P |= PPC_BRD31;

    return;
# endif
#endif

#if defined(__APPLE__) && defined(__MACH__)
    xdig_ppccap_P |= PPC_FPU;

    {
        int val;
        size_t len = sizeof(val);

        if (sysctlbyname("hw.optional.64bitops", &val, &len, NULL, 0) == 0) {
            if (val)
                xdig_ppccap_P |= PPC_FPU64;
        }

        len = sizeof(val);
        if (sysctlbyname("hw.optional.altivec", &val, &len, NULL, 0) == 0) {
            if (val)
                xdig_ppccap_P |= PPC_ALTIVEC;
        }

        return;
    }
#endif

#ifdef OSSL_IMPLEMENT_GETAUXVAL
    {
        unsigned long hwcap = getauxval(AT_HWCAP);
        unsigned long hwcap2 = getauxval(AT_HWCAP2);

        if (hwcap & HWCAP_FPU) {
            xdig_ppccap_P |= PPC_FPU;

            if (sizeof(size_t) == 4) {
                /* In 32-bit case PPC_FPU64 is always fastest [if option] */
                if (hwcap & HWCAP_PPC64)
                    xdig_ppccap_P |= PPC_FPU64;
            } else {
                /* In 64-bit case PPC_FPU64 is fastest only on POWER6 */
                if (hwcap & HWCAP_POWER6_EXT)
                    xdig_ppccap_P |= PPC_FPU64;
            }
        }

        if (hwcap & HWCAP_ALTIVEC) {
            xdig_ppccap_P |= PPC_ALTIVEC;

            if ((hwcap & HWCAP_VSX) && (hwcap2 & HWCAP_VEC_CRYPTO))
                xdig_ppccap_P |= PPC_CRYPTO207;
        }

        if (hwcap2 & HWCAP_ARCH_3_00) {
            xdig_ppccap_P |= PPC_MADD300;
        }

        if (hwcap2 & HWCAP_ARCH_3_1) {
            xdig_ppccap_P |= PPC_BRD31;
        }
    }
#endif

    sigfillset(&all_masked);
    sigdelset(&all_masked, SIGILL);
    sigdelset(&all_masked, SIGTRAP);
#ifdef SIGEMT
    sigdelset(&all_masked, SIGEMT);
#endif
    sigdelset(&all_masked, SIGFPE);
    sigdelset(&all_masked, SIGBUS);
    sigdelset(&all_masked, SIGSEGV);

    memset(&ill_act, 0, sizeof(ill_act));
    ill_act.sa_handler = ill_handler;
    ill_act.sa_mask = all_masked;

    sigprocmask(SIG_SETMASK, &ill_act.sa_mask, &oset);
    sigaction(SIGILL, &ill_act, &ill_oact);

#ifndef OSSL_IMPLEMENT_GETAUXVAL
    if (sigsetjmp(ill_jmp, 1) == 0) {
        xdig_fpu_probe();
        xdig_ppccap_P |= PPC_FPU;

        if (sizeof(size_t) == 4) {
# ifdef __linux
            struct utsname uts;
            if (uname(&uts) == 0 && strcmp(uts.machine, "ppc64") == 0)
# endif
                if (sigsetjmp(ill_jmp, 1) == 0) {
                    xdig_ppc64_probe();
                    xdig_ppccap_P |= PPC_FPU64;
                }
        } else {
            /*
             * Wanted code detecting POWER6 CPU and setting PPC_FPU64
             */
        }
    }

    if (sigsetjmp(ill_jmp, 1) == 0) {
        xdig_altivec_probe();
        xdig_ppccap_P |= PPC_ALTIVEC;
        if (sigsetjmp(ill_jmp, 1) == 0) {
            xdig_crypto207_probe();
            xdig_ppccap_P |= PPC_CRYPTO207;
        }
    }

    if (sigsetjmp(ill_jmp, 1) == 0) {
        xdig_madd300_probe();
        xdig_ppccap_P |= PPC_MADD300;
    }
#endif

    if (sigsetjmp(ill_jmp, 1) == 0) {
        xdig_rdtsc_mftb();
        xdig_ppccap_P |= PPC_MFTB;
    } else if (sigsetjmp(ill_jmp, 1) == 0) {
        xdig_rdtsc_mfspr268();
        xdig_ppccap_P |= PPC_MFSPR268;
    }

    sigaction(SIGILL, &ill_oact, NULL);
    sigprocmask(SIG_SETMASK, &oset, NULL);
}

#else /* xdig_CPUID_OBJ */

#endif /* ! xdig_CPUID_OBJ */
#endif /* ppc */
