#include "internal/sha.h"

#include "sha_local.h"

unsigned char *xdig_sha1(const unsigned char *d, size_t n, unsigned char *md)
{
    static unsigned char m[XDIG_SHA_DIGEST_LENGTH];
    xdig_sha1_ctx_t ctx;

    if (md == NULL)
        md = m;

    xdig_sha1_init(&ctx);
    xdig_sha1_update(&ctx, d, n);
    xdig_sha1_final(md, &ctx);

    return md;
}
