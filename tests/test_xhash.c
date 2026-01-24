#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stdint.h>

#include <xhash/sha1.h>
#include <xhash/xhash.h>

#include "bench_base.h"

int setup_tests(void);

int main()
{
    xhash_sha1_ctx_t *ctx = malloc(XHASH_SHA1_CTX_SIZE);
    xhash_bench__ctx_t *bench;

    unsigned char digest[XHASH_SHA1_DIGEST_LENGTH];

    xhash_init();

    xhash_sha1_init(ctx);

    bench = xhash_bench__ctx_create("SHA1", 1.0, 1024 * 1024);

    while (xhash_bench__ctx_next(bench))
    {
        xhash_sha1_update(ctx, bench->buf, bench->bufsize);
    }

    xhash_sha1_final(digest, ctx);

    xhash_bench__ctx_finish(bench);
    xhash_bench__ctx_free(bench);

    setup_tests();

    return 0;
}

