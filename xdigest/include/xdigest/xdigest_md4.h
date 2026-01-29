#ifndef XDIG_MD4_H
#define XDIG_MD4_H

#pragma once

#include <stddef.h> /* for size_t */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*
 * MD4
 */
typedef struct xdig_md4_ctx_t xdig_md4_ctx_t;

size_t xdig_md4_ctx_size(void);

#define XDIG_MD4_DIGEST_LENGTH 16

int xdig_md4_init(xdig_md4_ctx_t *ctx);

int xdig_md4_update(xdig_md4_ctx_t *ctx,
                    const void *data,
                    size_t len);

int xdig_md4_final(unsigned char *digest,
                   xdig_md4_ctx_t *ctx);

unsigned char *
xdig_md4(const void *data,
         size_t len,
         unsigned char *digest);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
