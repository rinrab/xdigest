/*
 * Copyright 2004-2024 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the Apache License 2.0 (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */

#include <stdlib.h>
#include <string.h>

#include <xdigest/xdigest_sha2.h>
#include "crypto/sha.h"

#include "sha256_local.h"

/* SHA256 */
void xdig_sha256_ctx_init(xdig_sha256_ctx_t *ctx)
{
    memset(ctx, 0, sizeof(*ctx));
    ctx->h[0] = 0x6a09e667UL;
    ctx->h[1] = 0xbb67ae85UL;
    ctx->h[2] = 0x3c6ef372UL;
    ctx->h[3] = 0xa54ff53aUL;
    ctx->h[4] = 0x510e527fUL;
    ctx->h[5] = 0x9b05688cUL;
    ctx->h[6] = 0x1f83d9abUL;
    ctx->h[7] = 0x5be0cd19UL;
    ctx->md_len = XDIG_SHA256_DIGEST_LENGTH;
}

unsigned char *xdig_sha256(unsigned char *digest, const void *data,
                           size_t len)
{
    xdig_sha256_ctx_t ctx;

    xdig_sha256_ctx_init(&ctx);
    xdig_sha256_ctx_update(&ctx, data, len);
    xdig_sha256_ctx_final(&ctx, digest);

    return digest;
}

size_t xdig_sha256_ctx_size(void)
{
    return sizeof(xdig_sha256_ctx_t);
}

/* SHA224 */
struct xdig_sha224_ctx_t {
    xdig_sha256_ctx_t state;
};

void xdig_sha224_ctx_init(xdig_sha224_ctx_t *ctx)
{
    memset(ctx, 0, sizeof(*ctx));
    ctx->state.h[0] = 0xc1059ed8UL;
    ctx->state.h[1] = 0x367cd507UL;
    ctx->state.h[2] = 0x3070dd17UL;
    ctx->state.h[3] = 0xf70e5939UL;
    ctx->state.h[4] = 0xffc00b31UL;
    ctx->state.h[5] = 0x68581511UL;
    ctx->state.h[6] = 0x64f98fa7UL;
    ctx->state.h[7] = 0xbefa4fa4UL;
    ctx->state.md_len = XDIG_SHA224_DIGEST_LENGTH;
}


void xdig_sha224_ctx_update(xdig_sha224_ctx_t *ctx, const void *data,
                            size_t len)
{
    xdig_sha256_ctx_update(&ctx->state, data, len);
}

void xdig_sha224_ctx_final(xdig_sha224_ctx_t *ctx, unsigned char *digest)
{
    xdig_sha256_ctx_final(&ctx->state, digest);
}

unsigned char *xdig_sha224(unsigned char *digesdt, const void *data,
                           size_t len)
{
    xdig_sha224_ctx_t ctx;

    xdig_sha224_ctx_init(&ctx);
    xdig_sha224_ctx_update(&ctx, data, len);
    xdig_sha224_ctx_final(&ctx, digesdt);

    return digesdt;
}

size_t xdig_sha224_ctx_size(void)
{
    return sizeof(xdig_sha224_ctx_t);
}

/* SHA256_192 */
void ossl_sha256_192_init(xdig_sha256_ctx_t *ctx)
{
    xdig_sha256_ctx_init(ctx);
    ctx->md_len = XDIG_SHA256_192_DIGEST_LENGTH;
}
