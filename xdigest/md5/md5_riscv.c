/*
 * md5/md5_riscv.c: riscv interface for MD5 asm
 *
 * ====================================================================
 * Copyright 2025 The OpenSSL Project Authors. All Rights Reserved.
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

#include "xdigest/xdigest_md5.h"
#include "crypto/riscv_arch.h"

void ossl_md5_block_asm_data_order(xdig_md5_ctx_t *c, const void *p, size_t num);
void ossl_md5_block_asm_data_order_zbb(xdig_md5_ctx_t *c, const void *p, size_t num);
void ossl_md5_block_asm_data_order_riscv64(xdig_md5_ctx_t *c, const void *p, size_t num);
void ossl_md5_block_asm_data_order(xdig_md5_ctx_t *c, const void *p, size_t num)
{
    if (RISCV_HAS_ZBB()) {
        ossl_md5_block_asm_data_order_zbb(c, p, num);
    } else {
        ossl_md5_block_asm_data_order_riscv64(c, p, num);
    }
}

#endif /* __riscv */
