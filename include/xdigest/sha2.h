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
#define XDIG_SHA224_CTX_SIZE 112
typedef struct xdig_sha224_ctx_t xdig_sha224_ctx_t;

#define XDIG_SHA224_DIGEST_LENGTH 28

int xdig_sha224_init(xdig_sha224_ctx_t *ctx);

int xdig_sha224_update(xdig_sha224_ctx_t *ctx,
                       const void *data,
                       size_t len);

int xdig_sha224_final(unsigned char *digest,
                      xdig_sha224_ctx_t *ctx);

void xdig_sha224(const unsigned char *data,
                 size_t len,
                 unsigned char *digest);

/*
 * SHA256
 */
#define XDIG_SHA256_CTX_SIZE 112
typedef struct xdig_sha256_ctx_t xdig_sha256_ctx_t;

#define XDIG_SHA256_DIGEST_LENGTH 32

int xdig_sha256_init(xdig_sha256_ctx_t *ctx);

int xdig_sha256_update(xdig_sha256_ctx_t *ctx,
                       const void *data,
                       size_t len);

int xdig_sha256_final(unsigned char *digest,
                      xdig_sha256_ctx_t *ctx);

void xdig_sha256(const unsigned char *data,
                 size_t len,
                 unsigned char *digest);

#define XDIG_SHA256_192_DIGEST_LENGTH 24

/*
 * SHA384
 */
#define XDIG_SHA384_CTX_SIZE 216
typedef struct xdig_sha384_ctx_t xdig_sha384_ctx_t;

#define XDIG_SHA384_DIGEST_LENGTH 48

int xdig_sha384_init(xdig_sha384_ctx_t *ctx);

int xdig_sha384_update(xdig_sha384_ctx_t *ctx,
                       const void *data,
                       size_t len);

int xdig_sha384_final(unsigned char *digest,
                      xdig_sha384_ctx_t *ctx);

void xdig_sha384(const unsigned char *data,
                 size_t len,
                 unsigned char *digest);

/*
 * SHA512
 */
#define XDIG_SHA512_CTX_SIZE 216
typedef struct xdig_sha512_ctx_t xdig_sha512_ctx_t;

#define XDIG_SHA512_DIGEST_LENGTH 64

int xdig_sha512_init(xdig_sha512_ctx_t *ctx);

int xdig_sha512_update(xdig_sha512_ctx_t *ctx,
                       const void *data,
                       size_t len);

int xdig_sha512_final(unsigned char *digest,
                      xdig_sha512_ctx_t *ctx);

void xdig_sha512(const unsigned char *data,
                 size_t len,
                 unsigned char *digest);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
