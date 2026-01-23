#ifndef XHASH_SHA1_H
#define XHASH_SHA1_H

#pragma once

#include <stddef.h> /* for size_t */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*
 * SHA1
 */
#define XHASH_SHA1_CTX_SIZE 96
typedef struct xhash_sha1_ctx_t xhash_sha1_ctx_t;

#define XHASH_SHA1_DIGEST_LENGTH 20

int xhash_sha1_init(xhash_sha1_ctx_t *p_ctx);

int xhash_sha1_update(xhash_sha1_ctx_t *ctx,
                      const void *data,
                      size_t len);

int xhash_sha1_final(unsigned char *p_digest,
                     xhash_sha1_ctx_t *ctx);

void xhash_sha1(const unsigned char *data,
                size_t len,
                unsigned char *p_digest);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
