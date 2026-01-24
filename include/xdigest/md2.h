#ifndef XDIG_MD2_H
#define XDIG_MD2_H

#pragma once

#include <stddef.h> /* for size_t */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*
 * MD2
 */
#define XDIG_MD2_CTX_SIZE 52
typedef struct xdig_md2_ctx_t xdig_md2_ctx_t;

#define XDIG_MD2_DIGEST_LENGTH 16

int xdig_md2_init(xdig_md2_ctx_t *ctx);

int xdig_md2_update(xdig_md2_ctx_t *ctx,
                     const void *data,
                     size_t len);

int xdig_md2_final(unsigned char *digest,
                    xdig_md2_ctx_t *ctx);

void xdig_md2(const unsigned char *data,
               size_t len,
               unsigned char *digest);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
