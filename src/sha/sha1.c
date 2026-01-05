#include <xhash/sha.h>

#include "sha_local.h"

unsigned char *xhash_sha1(const unsigned char *d, size_t n, unsigned char *md)
{
    static unsigned char m[SHA_DIGEST_LENGTH];
    xhash_sha1_ctx_t ctx;

    if (md == NULL)
        md = m;

    xhash_sha1_init(&ctx);
    xhash_sha1_update(&ctx, d, n);
    xhash_sha1_final(md, &ctx);

    return md;
}
