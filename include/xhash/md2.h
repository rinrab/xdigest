#ifndef XHASH_MD2_H
#define XHASH_MD2_H

#pragma once

#include <stddef.h> /* for size_t */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*
 * MD2
 */
#define XHASH_MD2_CTX_SIZE 52
typedef struct xhash_md2_ctx_t xhash_md2_ctx_t;

#define XHASH_MD2_DIGEST_LENGTH 16

int xhash_md2_init(xhash_md2_ctx_t *ctx);

int xhash_md2_update(xhash_md2_ctx_t *ctx,
                     const void *data,
                     size_t len);

int xhash_md2_final(unsigned char *digest,
                    xhash_md2_ctx_t *ctx);

void xhash_md2(const unsigned char *data,
               size_t len,
               unsigned char *digest);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
