#include "internal/sha.h"

#include "sha_local.h"

unsigned char *xdig_sha1(unsigned char *digest, const void *data, size_t len)
{
    xdig_sha1_ctx_t ctx;

    xdig_sha1_ctx_init(&ctx);
    xdig_sha1_ctx_update(&ctx, data, len);
    xdig_sha1_ctx_final(&ctx, digest);

    return digest;
}

size_t xdig_sha1_ctx_size(void)
{
    return sizeof(xdig_sha1_ctx_t);
}
