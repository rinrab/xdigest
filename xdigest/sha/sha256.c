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
void xdig_sha256_ctx_init(xdig_sha256_ctx_t *c)
{
    memset(c, 0, sizeof(*c));
    c->h[0] = 0x6a09e667UL;
    c->h[1] = 0xbb67ae85UL;
    c->h[2] = 0x3c6ef372UL;
    c->h[3] = 0xa54ff53aUL;
    c->h[4] = 0x510e527fUL;
    c->h[5] = 0x9b05688cUL;
    c->h[6] = 0x1f83d9abUL;
    c->h[7] = 0x5be0cd19UL;
    c->md_len = XDIG_SHA256_DIGEST_LENGTH;
}

unsigned char *xdig_sha256(const void *d, size_t n, unsigned char *md)
{
    xdig_sha256_ctx_t ctx;

    xdig_sha256_ctx_init(&ctx);
    xdig_sha256_ctx_update(&ctx, d, n);
    xdig_sha256_ctx_final(&ctx, md);

    return md;
}

size_t xdig_sha256_ctx_size(void)
{
    return sizeof(xdig_sha256_ctx_t);
}

/* SHA224 */
struct xdig_sha224_ctx_t {
    xdig_sha256_ctx_t state;
};

void xdig_sha224_ctx_init(xdig_sha224_ctx_t *c)
{
    memset(c, 0, sizeof(*c));
    c->state.h[0] = 0xc1059ed8UL;
    c->state.h[1] = 0x367cd507UL;
    c->state.h[2] = 0x3070dd17UL;
    c->state.h[3] = 0xf70e5939UL;
    c->state.h[4] = 0xffc00b31UL;
    c->state.h[5] = 0x68581511UL;
    c->state.h[6] = 0x64f98fa7UL;
    c->state.h[7] = 0xbefa4fa4UL;
    c->state.md_len = XDIG_SHA224_DIGEST_LENGTH;
}


void xdig_sha224_ctx_update(xdig_sha224_ctx_t *c, const void *data, size_t len)
{
    xdig_sha256_ctx_update(&c->state, data, len);
}

void xdig_sha224_ctx_final(xdig_sha224_ctx_t *c, unsigned char *md)
{
    xdig_sha256_ctx_final(&c->state, md);
}

unsigned char *xdig_sha224(const void *d, size_t n, unsigned char *md)
{
    xdig_sha224_ctx_t ctx;

    xdig_sha224_ctx_init(&ctx);
    xdig_sha224_ctx_update(&ctx, d, n);
    xdig_sha224_ctx_final(&ctx, md);

    return md;
}

size_t xdig_sha224_ctx_size(void)
{
    return sizeof(xdig_sha224_ctx_t);
}

/* SHA256_192 */
void ossl_sha256_192_init(xdig_sha256_ctx_t *c)
{
    xdig_sha256_ctx_init(c);
    c->md_len = XDIG_SHA256_192_DIGEST_LENGTH;
}
