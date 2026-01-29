#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stdint.h>

#include <xdigest/xdigest_sha1.h>
#include <xdigest/xdigest_sha2.h>
#include <xdigest/xdigest_md5.h>
#include <xdigest/xdigest_md4.h>
#include <xdigest/xdigest_md2.h>
#include <xdigest/xdigest.h>

#include "bench_base.h"

int setup_tests(void);

/* it's lowkey the best peace of code i've ever wrote in my life */
#define ADD_ALGO(name) \
    do { \
        xdig_##name##_ctx_t *ctx = malloc(xdig_##name##_ctx_size()); \
        xdig_bench__ctx_t *bench; \
        unsigned char digest[256]; \
        xdig_##name##_init(ctx); \
        bench = xdig_bench__ctx_create(#name, 1.0, 1024 * 1024); \
        while (xdig_bench__ctx_next(bench)) { \
            xdig_##name##_update(ctx, bench->buf, bench->bufsize); \
        } \
        xdig_##name##_final(digest, ctx); \
        xdig_bench__ctx_finish(bench); \
        xdig_bench__ctx_free(bench); \
        free(ctx); \
    } while (0);

int main()
{
    xdig_init();

    ADD_ALGO(sha1);
    ADD_ALGO(sha224);
    ADD_ALGO(sha256);
    ADD_ALGO(sha384);
    ADD_ALGO(sha512);
    ADD_ALGO(md5);
    ADD_ALGO(md4);
    ADD_ALGO(md2);

    setup_tests();

    return 0;
}

