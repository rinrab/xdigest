/*
 * @copyright
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
 * @endcopyright
 *
 * @file xdigest_md4.h
 * @brief MD4 algorithm routines provided by xdigest
 */

#ifndef XDIG_MD4_H
#define XDIG_MD4_H

#pragma once

#include <stddef.h> /* for size_t */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*
 * MD4
 */
typedef struct xdig_md4_ctx_t xdig_md4_ctx_t;

size_t xdig_md4_ctx_size(void);

#define XDIG_MD4_DIGEST_LENGTH 16

int xdig_md4_ctx_init(xdig_md4_ctx_t *ctx);

int xdig_md4_ctx_update(xdig_md4_ctx_t *ctx,
                        const void *data,
                        size_t len);

int xdig_md4_ctx_final(xdig_md4_ctx_t *ctx,
                       unsigned char *digest);

unsigned char *
xdig_md4(unsigned char *digest,
         const void *data,
         size_t len);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
