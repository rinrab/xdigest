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

#include "sha512_local.h"

struct xdig_sha384_ctx_t {
    xdig_sha512_ctx_t state;
};

#if 0
void sha512_224_init(xdig_sha512_ctx_t *ctx)
{
    ctx->h[0] = U64(0x8c3d37c819544da2);
    ctx->h[1] = U64(0x73e1996689dcd4d6);
    ctx->h[2] = U64(0x1dfab7ae32ff9c82);
    ctx->h[3] = U64(0x679dd514582f9fcf);
    ctx->h[4] = U64(0x0f6d2b697bd44da8);
    ctx->h[5] = U64(0x77e36f7304c48942);
    ctx->h[6] = U64(0x3f9d85a86a1d36c8);
    ctx->h[7] = U64(0x1112e6ad91d692a1);

    ctx->Nl = 0;
    ctx->Nh = 0;
    ctx->num = 0;
    ctx->md_len = XDIG_SHA224_DIGEST_LENGTH;
}

void sha512_256_init(xdig_sha512_ctx_t *ctx)
{
    ctx->h[0] = U64(0x22312194fc2bf72c);
    ctx->h[1] = U64(0x9f555fa3c84c64c2);
    ctx->h[2] = U64(0x2393b86b6f53b151);
    ctx->h[3] = U64(0x963877195940eabd);
    ctx->h[4] = U64(0x96283ee2a88effe3);
    ctx->h[5] = U64(0xbe5e1e2553863992);
    ctx->h[6] = U64(0x2b0199fc2c85b8aa);
    ctx->h[7] = U64(0x0eb72ddc81c52ca2);

    ctx->Nl = 0;
    ctx->Nh = 0;
    ctx->num = 0;
    ctx->md_len = XDIG_SHA256_DIGEST_LENGTH;
}
#endif

void xdig_sha384_ctx_init(xdig_sha384_ctx_t *ctx)
{
    ctx->state.h[0] = U64(0xcbbb9d5dc1059ed8);
    ctx->state.h[1] = U64(0x629a292a367cd507);
    ctx->state.h[2] = U64(0x9159015a3070dd17);
    ctx->state.h[3] = U64(0x152fecd8f70e5939);
    ctx->state.h[4] = U64(0x67332667ffc00b31);
    ctx->state.h[5] = U64(0x8eb44a8768581511);
    ctx->state.h[6] = U64(0xdb0c2e0d64f98fa7);
    ctx->state.h[7] = U64(0x47b5481dbefa4fa4);

    ctx->state.Nl = 0;
    ctx->state.Nh = 0;
    ctx->state.num = 0;
    ctx->state.md_len = XDIG_SHA384_DIGEST_LENGTH;
}

unsigned char *xdig_sha384(unsigned char *digest, const void *data,
                           size_t len)
{
    xdig_sha384_ctx_t ctx;

    xdig_sha384_ctx_init(&ctx);
    xdig_sha384_ctx_update(&ctx, data, len);
    xdig_sha384_ctx_final(&ctx, digest);

    return digest;
}

void xdig_sha512_ctx_init(xdig_sha512_ctx_t *ctx)
{
    ctx->h[0] = U64(0x6a09e667f3bcc908);
    ctx->h[1] = U64(0xbb67ae8584caa73b);
    ctx->h[2] = U64(0x3c6ef372fe94f82b);
    ctx->h[3] = U64(0xa54ff53a5f1d36f1);
    ctx->h[4] = U64(0x510e527fade682d1);
    ctx->h[5] = U64(0x9b05688c2b3e6c1f);
    ctx->h[6] = U64(0x1f83d9abfb41bd6b);
    ctx->h[7] = U64(0x5be0cd19137e2179);

    ctx->Nl = 0;
    ctx->Nh = 0;
    ctx->num = 0;
    ctx->md_len = XDIG_SHA512_DIGEST_LENGTH;
}

unsigned char *xdig_sha512(unsigned char *digest, const void *data,
                           size_t len)
{
    xdig_sha512_ctx_t ctx;

    xdig_sha512_ctx_init(&ctx);
    xdig_sha512_ctx_update(&ctx, data, len);
    xdig_sha512_ctx_final(&ctx, digest);

    return digest;
}

size_t xdig_sha512_ctx_size(void)
{
    return sizeof(xdig_sha512_ctx_t);
}

size_t xdig_sha384_ctx_size(void)
{
    return xdig_sha512_ctx_size();
}

void xdig_sha384_ctx_final(xdig_sha384_ctx_t *ctx, unsigned char *digest)
{
    xdig_sha512_ctx_final(&ctx->state, digest);
}

void xdig_sha384_ctx_update(xdig_sha384_ctx_t *ctx, const void *data, size_t len)
{
    xdig_sha512_ctx_update(&ctx->state, data, len);
}

