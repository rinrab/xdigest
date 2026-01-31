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
 * @file xdigest_sha2.h
 * @brief SHA2 algorithm routines provided by xdigest
 */

#ifndef XDIG_SHA2_H
#define XDIG_SHA2_H

#pragma once

#include <stddef.h> /* for size_t */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*
 * SHA224
 */
typedef struct xdig_sha224_ctx_t xdig_sha224_ctx_t;

#define XDIG_SHA224_DIGEST_LENGTH 28

size_t xdig_sha224_ctx_size(void);

int xdig_sha224_ctx_init(xdig_sha224_ctx_t *ctx);

int xdig_sha224_ctx_update(xdig_sha224_ctx_t *ctx,
                           const void *data,
                           size_t len);

int xdig_sha224_ctx_final(xdig_sha224_ctx_t *ctx,
                          unsigned char *digest);

unsigned char *
xdig_sha224(const void *data,
            size_t len,
            unsigned char *digest);

/*
 * SHA256
 */
typedef struct xdig_sha256_ctx_t xdig_sha256_ctx_t;

size_t xdig_sha256_ctx_size(void);

#define XDIG_SHA256_DIGEST_LENGTH 32

int xdig_sha256_ctx_init(xdig_sha256_ctx_t *ctx);

int xdig_sha256_ctx_update(xdig_sha256_ctx_t *ctx,
                           const void *data,
                           size_t len);

int xdig_sha256_ctx_final(xdig_sha256_ctx_t *ctx,
                          unsigned char *digest);

unsigned char *
xdig_sha256(const void *data,
            size_t len,
            unsigned char *digest);

#define XDIG_SHA256_192_DIGEST_LENGTH 24

/*
 * SHA384
 */
typedef struct xdig_sha384_ctx_t xdig_sha384_ctx_t;

size_t xdig_sha384_ctx_size(void);

#define XDIG_SHA384_DIGEST_LENGTH 48

int xdig_sha384_ctx_init(xdig_sha384_ctx_t *ctx);

int xdig_sha384_ctx_update(xdig_sha384_ctx_t *ctx,
                           const void *data,
                           size_t len);

int xdig_sha384_ctx_final(xdig_sha384_ctx_t *ctx,
                          unsigned char *digest);

unsigned char *
xdig_sha384(const void *data,
            size_t len,
            unsigned char *digest);

/*
 * SHA512
 */
typedef struct xdig_sha512_ctx_t xdig_sha512_ctx_t;

size_t xdig_sha512_ctx_size(void);

#define XDIG_SHA512_DIGEST_LENGTH 64

int xdig_sha512_ctx_init(xdig_sha512_ctx_t *ctx);

int xdig_sha512_ctx_update(xdig_sha512_ctx_t *ctx,
                           const void *data,
                           size_t len);

int xdig_sha512_ctx_final(xdig_sha512_ctx_t *ctx,
                          unsigned char *digest);

unsigned char *
xdig_sha512(const void *data,
            size_t len,
            unsigned char *digest);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
