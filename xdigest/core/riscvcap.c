/*
 * core/riscvcap.c: CPU initialization for riscv processors
 *
 * ====================================================================
 * Copyright 2022-2025 The OpenSSL Project Authors. All Rights Reserved.
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

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define xdig_RISCVCAP_IMPL
#include "crypto/riscv_arch.h"

#ifdef OSSL_RISCV_HWPROBE
#include <unistd.h>
#include <sys/syscall.h>
#include <asm/hwprobe.h>
#include <sys/auxv.h>
#endif

extern size_t riscv_vlen_asm(void);

static size_t vlen = 0;

#ifdef OSSL_RISCV_HWPROBE
unsigned int xdig_riscv_hwcap_P = 0;
#endif

size_t xdig_instrument_bus(unsigned int *out, size_t cnt)
{
    return 0;
}

size_t xdig_instrument_bus2(unsigned int *out, size_t cnt, size_t max)
{
    return 0;
}

#ifdef OSSL_RISCV_HWPROBE
static long riscv_hwprobe(struct riscv_hwprobe *pairs, size_t pair_count,
    size_t cpu_count, unsigned long *cpus,
    unsigned int flags)
{
    return syscall(__NR_riscv_hwprobe, pairs, pair_count, cpu_count, cpus, flags);
}

static void hwprobe_to_cap(void)
{
    long ret;
    struct riscv_hwprobe pairs[OSSL_RISCV_HWPROBE_PAIR_COUNT] = {
        OSSL_RISCV_HWPROBE_PAIR_CONTENT
    };

    ret = riscv_hwprobe(pairs, OSSL_RISCV_HWPROBE_PAIR_COUNT, 0, NULL, 0);
    /* if hwprobe syscall does not exist, ret would be -ENOSYS */
    if (ret == 0) {
        for (size_t i = 0; i < kRISCVNumCaps; ++i) {
            for (size_t j = 0; j != OSSL_RISCV_HWPROBE_PAIR_COUNT; ++j) {
                if (pairs[j].key == RISCV_capabilities[i].hwprobe_key
                    && (pairs[j].value & RISCV_capabilities[i].hwprobe_value)
                        != 0)
                    if (!IS_IN_DEPEND_VECTOR(RISCV_capabilities[i].bit_offset) || VECTOR_CAPABLE)
                        /* Match, set relevant bit in xdig_riscvcap_P[] */
                        xdig_riscvcap_P[RISCV_capabilities[i].index] |= (1 << RISCV_capabilities[i].bit_offset);
            }
        }
    }
}
#endif /* OSSL_RISCV_HWPROBE */

size_t riscv_vlen(void)
{
    return vlen;
}

void xdig_cpuid_setup(void)
{
    char *e;
    static int trigger = 0;

    if (trigger != 0)
        return;
    trigger = 1;

#ifdef OSSL_RISCV_HWPROBE
    else {
        xdig_riscv_hwcap_P = getauxval(AT_HWCAP);
        hwprobe_to_cap();
    }
#endif

    if (RISCV_HAS_V()) {
        vlen = riscv_vlen_asm();
    }
}

/* public interface */
void xdig_init()
{
    xdig_cpuid_setup();
}
