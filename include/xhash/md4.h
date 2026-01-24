#ifndef XHASH_MD4_H
#define XHASH_MD4_H

#pragma once

#include <stddef.h> /* for size_t */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*
 * MD4
 */
#define XHASH_MD4_CTX_SIZE 92
typedef struct xhash_md4_ctx_t xhash_md4_ctx_t;

#define XHASH_MD4_DIGEST_LENGTH 16

int xhash_md4_init(xhash_md4_ctx_t *ctx);

int xhash_md4_update(xhash_md4_ctx_t *ctx,
                     const void *data,
                     size_t len);

int xhash_md4_final(unsigned char *digest,
                    xhash_md4_ctx_t *ctx);

void xhash_md4(const unsigned char *data,
               size_t len,
               unsigned char *digest);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
