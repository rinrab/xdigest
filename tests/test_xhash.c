#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stdint.h>

#include <xhash/sha1.h>
#include <xhash/sha2.h>
#include <xhash/md5.h>
#include <xhash/md4.h>
#include <xhash/md2.h>
#include <xhash/xhash.h>

#include "bench_base.h"

int setup_tests(void);

/* it's lowkey the best peace of code i've ever wrote in my life */
#define ADD_ALGO(name) \
    do { \
        xhash_##name##_ctx_t *ctx = malloc(256); \
        xhash_bench__ctx_t *bench; \
        unsigned char digest[256]; \
        xhash_##name##_init(ctx); \
        bench = xhash_bench__ctx_create(#name, 1.0, 1024 * 1024); \
        while (xhash_bench__ctx_next(bench)) { \
            xhash_##name##_update(ctx, bench->buf, bench->bufsize); \
        } \
        xhash_##name##_final(digest, ctx); \
        xhash_bench__ctx_finish(bench); \
        xhash_bench__ctx_free(bench); \
        free(ctx); \
    } while (0);

int main()
{
    xhash_init();

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

