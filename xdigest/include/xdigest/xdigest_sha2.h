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

int xdig_sha224_ctx_final(unsigned char *digest,
                          xdig_sha224_ctx_t *ctx);

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

int xdig_sha256_ctx_final(unsigned char *digest,
                          xdig_sha256_ctx_t *ctx);

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

int xdig_sha384_ctx_final(unsigned char *digest,
                          xdig_sha384_ctx_t *ctx);

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

int xdig_sha512_ctx_final(unsigned char *digest,
                          xdig_sha512_ctx_t *ctx);

unsigned char *
xdig_sha512(const void *data,
            size_t len,
            unsigned char *digest);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
