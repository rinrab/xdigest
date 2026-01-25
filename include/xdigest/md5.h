#ifndef XDIG_MD5_H
#define XDIG_MD5_H

#pragma once

#include <stddef.h> /* for size_t */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*
 * MD5
 */
#define XDIG_MD5_CTX_SIZE 92
typedef struct xdig_md5_ctx_t xdig_md5_ctx_t;

#define XDIG_MD5_DIGEST_LENGTH 16

int xdig_md5_init(xdig_md5_ctx_t *ctx);

int xdig_md5_update(xdig_md5_ctx_t *ctx,
                    const void *data,
                    size_t len);

int xdig_md5_final(unsigned char *digest,
                   xdig_md5_ctx_t *ctx);

void xdig_md5(const unsigned char *data,
              size_t len,
              unsigned char *digest);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
