#ifndef XHASH_SHA2_H
#define XHASH_SHA2_H

#pragma once

#include <stddef.h> /* for size_t */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*
 * SHA224
 */
#define XHASH_SHA224_CTX_SIZE 112
typedef struct xhash_sha224_ctx_t xhash_sha224_ctx_t;

#define XHASH_SHA224_DIGEST_LENGTH 28

int xhash_sha224_init(xhash_sha224_ctx_t *ctx);

int xhash_sha224_update(xhash_sha224_ctx_t *ctx,
                        const void *data,
                        size_t len);

int xhash_sha224_final(unsigned char *digest,
                       xhash_sha224_ctx_t *ctx);

void xhash_sha224(const unsigned char *data,
                  size_t len,
                  unsigned char *digest);

/*
 * SHA256
 */
#define XHASH_SHA256_CTX_SIZE 112
typedef struct xhash_sha256_ctx_t xhash_sha256_ctx_t;

#define XHASH_SHA256_DIGEST_LENGTH 32

int xhash_sha256_init(xhash_sha256_ctx_t *ctx);

int xhash_sha256_update(xhash_sha256_ctx_t *ctx,
                        const void *data,
                        size_t len);

int xhash_sha256_final(unsigned char *digest,
                       xhash_sha256_ctx_t *ctx);

void xhash_sha256(const unsigned char *data,
                  size_t len,
                  unsigned char *digest);

#define XHASH_SHA256_192_DIGEST_LENGTH 24

/*
 * SHA384
 */
#define XHASH_SHA384_CTX_SIZE 216
typedef struct xhash_sha384_ctx_t xhash_sha384_ctx_t;

#define XHASH_SHA384_DIGEST_LENGTH 48

int xhash_sha384_init(xhash_sha384_ctx_t *ctx);

int xhash_sha384_update(xhash_sha384_ctx_t *ctx,
                        const void *data,
                        size_t len);

int xhash_sha384_final(unsigned char *digest,
                       xhash_sha384_ctx_t *ctx);

void xhash_sha384(const unsigned char *data,
                  size_t len,
                  unsigned char *digest);

/*
 * SHA512
 */
#define XHASH_SHA512_CTX_SIZE 216
typedef struct xhash_sha512_ctx_t xhash_sha512_ctx_t;

#define XHASH_SHA512_DIGEST_LENGTH 64

int xhash_sha512_init(xhash_sha512_ctx_t *ctx);

int xhash_sha512_update(xhash_sha512_ctx_t *ctx,
                        const void *data,
                        size_t len);

int xhash_sha512_final(unsigned char *digest,
                       xhash_sha512_ctx_t *ctx);

void xhash_sha512(const unsigned char *data,
                  size_t len,
                  unsigned char *digest);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
