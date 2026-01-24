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
#define XDIG_SHA1_CTX_SIZE 96
typedef struct xdig_sha1_ctx_t xdig_sha1_ctx_t;

#define XDIG_SHA1_DIGEST_LENGTH 20

int xdig_sha1_init(xdig_sha1_ctx_t *ctx);

int xdig_sha1_update(xdig_sha1_ctx_t *ctx,
                      const void *data,
                      size_t len);

int xdig_sha1_final(unsigned char *digest,
                     xdig_sha1_ctx_t *ctx);

void xdig_sha1(const unsigned char *data,
                size_t len,
                unsigned char *digest);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
