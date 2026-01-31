/*
 * Copyright 2004-2024 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the Apache License 2.0 (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */

/*-
 * IMPLEMENTATION NOTES.
 *
 * As you might have noticed, 32-bit hash algorithms:
 *
 * - permit XDIG_SHA_LONG to be wider than 32-bit
 * - optimized versions implement two transform functions: one operating
 *   on [aligned] data in host byte order, and one operating on data in input
 *   stream byte order;
 * - share common byte-order neutral collector and padding function
 *   implementations, crypto/md32_common.h;
 *
 * Neither of the above applies to this SHA-512 implementation. Reasons
 * [in reverse order] are:
 *
 * - it's the only 64-bit hash algorithm for the moment of this writing,
 *   there is no need for common collector/padding implementation [yet];
 * - by supporting only one transform function [which operates on
 *   *aligned* data in input stream byte order, big-endian in this case]
 *   we minimize burden of maintenance in two ways: a) collector/padding
 *   function is simpler; b) only one transform function to stare at;
 * - XDIG_SHA_LONG64 is required to be exactly 64-bit in order to be able to
 *   apply a number of optimizations to mitigate potential performance
 *   penalties caused by previous design decision;
 *
 * Caveat lector.
 *
 * Implementation relies on the fact that "long long" is 64-bit on
 * both 32- and 64-bit platforms. If some compiler vendor comes up
 * with 128-bit long long, adjustment to sha.h would be required.
 * As this implementation relies on 64-bit integer type, it's totally
 * inappropriate for platforms which don't support it, most notably
 * 16-bit platforms.
 */
#include <stdlib.h>
#include <string.h>

#include "xdigest_sha2.h"
#include "crypto/sha.h"

#include "sha512_local.h"

struct xdig_sha384_ctx_t {
    xdig_sha512_ctx_t state;
};

void sha512_224_init(xdig_sha512_ctx_t *c)
{
    c->h[0] = U64(0x8c3d37c819544da2);
    c->h[1] = U64(0x73e1996689dcd4d6);
    c->h[2] = U64(0x1dfab7ae32ff9c82);
    c->h[3] = U64(0x679dd514582f9fcf);
    c->h[4] = U64(0x0f6d2b697bd44da8);
    c->h[5] = U64(0x77e36f7304c48942);
    c->h[6] = U64(0x3f9d85a86a1d36c8);
    c->h[7] = U64(0x1112e6ad91d692a1);

    c->Nl = 0;
    c->Nh = 0;
    c->num = 0;
    c->md_len = XDIG_SHA224_DIGEST_LENGTH;
}

void sha512_256_init(xdig_sha512_ctx_t *c)
{
    c->h[0] = U64(0x22312194fc2bf72c);
    c->h[1] = U64(0x9f555fa3c84c64c2);
    c->h[2] = U64(0x2393b86b6f53b151);
    c->h[3] = U64(0x963877195940eabd);
    c->h[4] = U64(0x96283ee2a88effe3);
    c->h[5] = U64(0xbe5e1e2553863992);
    c->h[6] = U64(0x2b0199fc2c85b8aa);
    c->h[7] = U64(0x0eb72ddc81c52ca2);

    c->Nl = 0;
    c->Nh = 0;
    c->num = 0;
    c->md_len = XDIG_SHA256_DIGEST_LENGTH;
}

void xdig_sha384_ctx_init(xdig_sha384_ctx_t *c)
{
    c->state.h[0] = U64(0xcbbb9d5dc1059ed8);
    c->state.h[1] = U64(0x629a292a367cd507);
    c->state.h[2] = U64(0x9159015a3070dd17);
    c->state.h[3] = U64(0x152fecd8f70e5939);
    c->state.h[4] = U64(0x67332667ffc00b31);
    c->state.h[5] = U64(0x8eb44a8768581511);
    c->state.h[6] = U64(0xdb0c2e0d64f98fa7);
    c->state.h[7] = U64(0x47b5481dbefa4fa4);

    c->state.Nl = 0;
    c->state.Nh = 0;
    c->state.num = 0;
    c->state.md_len = XDIG_SHA384_DIGEST_LENGTH;
}

unsigned char *xdig_sha384(const void *d, size_t n, unsigned char *md)
{
    xdig_sha384_ctx_t ctx;

    xdig_sha384_ctx_init(&ctx);
    xdig_sha384_ctx_update(&ctx, d, n);
    xdig_sha384_ctx_final(&ctx, md);

    return md;
}

void xdig_sha512_ctx_init(xdig_sha512_ctx_t *c)
{
    c->h[0] = U64(0x6a09e667f3bcc908);
    c->h[1] = U64(0xbb67ae8584caa73b);
    c->h[2] = U64(0x3c6ef372fe94f82b);
    c->h[3] = U64(0xa54ff53a5f1d36f1);
    c->h[4] = U64(0x510e527fade682d1);
    c->h[5] = U64(0x9b05688c2b3e6c1f);
    c->h[6] = U64(0x1f83d9abfb41bd6b);
    c->h[7] = U64(0x5be0cd19137e2179);

    c->Nl = 0;
    c->Nh = 0;
    c->num = 0;
    c->md_len = XDIG_SHA512_DIGEST_LENGTH;
}

unsigned char *xdig_sha512(const void *d, size_t n, unsigned char *md)
{
    xdig_sha512_ctx_t ctx;

    xdig_sha512_ctx_init(&ctx);
    xdig_sha512_ctx_update(&ctx, d, n);
    xdig_sha512_ctx_final(&ctx, md);

    return md;
}

size_t xdig_sha512_ctx_size(void)
{
    return sizeof(xdig_sha512_ctx_t);
}

size_t xdig_sha384_ctx_size(void)
{
    return xdig_sha512_ctx_size();
}

void xdig_sha384_ctx_final(xdig_sha384_ctx_t *c, unsigned char *md)
{
    xdig_sha512_ctx_final(&c->state, md);
}

void xdig_sha384_ctx_update(xdig_sha384_ctx_t *c, const void *data, size_t len)
{
    xdig_sha512_ctx_update(&c->state, data, len);
}

