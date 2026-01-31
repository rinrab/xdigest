#include "internal/sha.h"

#include "sha_local.h"

unsigned char *xdig_sha1(const unsigned char *data, size_t len, unsigned char *digest)
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
