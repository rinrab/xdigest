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
 * @file xdigest_sha1.h
 * @brief SHA1 algorithm routines provided by xdigest
 */

#ifndef XDIG_SHA1_H
#define XDIG_SHA1_H

#pragma once

#include <stddef.h> /* for size_t */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*
 * SHA1
 */
typedef struct xdig_sha1_ctx_t xdig_sha1_ctx_t;

size_t xdig_sha1_ctx_size(void);

#define XDIG_SHA1_DIGEST_LENGTH 20

int xdig_sha1_ctx_init(xdig_sha1_ctx_t *ctx);

int xdig_sha1_ctx_update(xdig_sha1_ctx_t *ctx,
                         const void *data,
                         size_t len);

int xdig_sha1_ctx_final(unsigned char *digest,
                        xdig_sha1_ctx_t *ctx);

unsigned char *
xdig_sha1(const void *data,
          size_t len,
          unsigned char *digest);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
