#ifndef XHASH_MD5_H
#define XHASH_MD5_H

#pragma once

#include <stddef.h> /* for size_t */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*
 * MD5
 */
#define XHASH_MD5_CTX_SIZE 92
typedef struct xhash_md5_ctx_t xhash_md5_ctx_t;

#define XHASH_MD5_DIGEST_LENGTH 16

int xhash_md5_init(xhash_md5_ctx_t *p_ctx);

int xhash_md5_update(xhash_md5_ctx_t *ctx,
                     const void *data,
                     size_t len);

int xhash_md5_final(unsigned char *p_digest,
                    xhash_md5_ctx_t *ctx);

void xhash_md5(const unsigned char *data,
               size_t len,
               unsigned char *p_digest);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
