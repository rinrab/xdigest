/*
 * sha/sha_riscv.c: riscv interface for SHA asm
 *
 * ====================================================================
 * Copyright 2023-2025 The OpenSSL Project Authors. All Rights Reserved.
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

#ifdef __riscv

#include <stdlib.h>
#include <string.h>

#include "xdigest/xdigest_sha2.h""
#include "crypto/riscv_arch.h"

void sha256_block_data_order_zvkb_zvknha_or_zvknhb(void *ctx, const void *in,
    size_t num);
void sha256_block_data_order_zbb(void *ctx, const void *in, size_t num);
void sha256_block_data_order_riscv64(void *ctx, const void *in, size_t num);
void sha256_block_data_order(xdig_sha256_ctx_t *ctx, const void *in, size_t num);

void sha256_block_data_order(xdig_sha256_ctx_t *ctx, const void *in, size_t num)
{
    if (RISCV_HAS_ZVKB() && (RISCV_HAS_ZVKNHA() || RISCV_HAS_ZVKNHB()) && riscv_vlen() >= 128) {
        sha256_block_data_order_zvkb_zvknha_or_zvknhb(ctx, in, num);
    } else if (RISCV_HAS_ZBB()) {
        sha256_block_data_order_zbb(ctx, in, num);
    } else {
        sha256_block_data_order_riscv64(ctx, in, num);
    }
}

void sha512_block_data_order_zvkb_zvknhb(void *ctx, const void *in, size_t num);
void sha512_block_data_order_zbb(void *ctx, const void *in, size_t num);
void sha512_block_data_order_c(void *ctx, const void *in, size_t num);
void sha512_block_data_order(xdig_sha512_ctx_t *ctx, const void *in, size_t num);

void sha512_block_data_order(xdig_sha512_ctx_t *ctx, const void *in, size_t num)
{
    if (RISCV_HAS_ZVKB_AND_ZVKNHB() && riscv_vlen() >= 128) {
        sha512_block_data_order_zvkb_zvknhb(ctx, in, num);
    } else if (RISCV_HAS_ZBB()) {
        sha512_block_data_order_zbb(ctx, in, num);
    } else {
        sha512_block_data_order_c(ctx, in, num);
    }
}

#endif /* __riscv */
