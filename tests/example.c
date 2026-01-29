#include <assert.h>
#include <string.h>
#include <stdlib.h>

#include <xdigest.h>
#include <xdigest_sha2.h>

int main() {
    xdig_sha256_ctx_t *ctx = malloc(XDIG_SHA256_CTX_SIZE);
    unsigned char digest1[XDIG_SHA256_DIGEST_LENGTH];
    unsigned char digest2[XDIG_SHA256_DIGEST_LENGTH];

    xdig_init();

    xdig_sha256_init(ctx);
    xdig_sha256_update(ctx, "abc", 3);
    xdig_sha256_update(ctx, "more things", sizeof("more things"));
    xdig_sha256_final(digest1, ctx);

    /* There is also a convenience wrapper. */
    xdig_sha256("abcmore things", sizeof("abcmore things"), digest2);

    /* Should be the same data. */
    assert(memcmp(digest1, digest2, sizeof(digest1)) == 0);

    return 0;
}
