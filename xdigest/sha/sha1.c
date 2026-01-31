#include "internal/sha.h"

#include "sha_local.h"

unsigned char *xdig_sha1(const unsigned char *d, size_t n, unsigned char *md)
{
    xdig_sha1_ctx_t ctx;

    xdig_sha1_ctx_init(&ctx);
    xdig_sha1_ctx_update(&ctx, d, n);
    xdig_sha1_ctx_final(&ctx, md);

    return md;
}

size_t xdig_sha1_ctx_size(void)
{
    return sizeof(xdig_sha1_ctx_t);
}
