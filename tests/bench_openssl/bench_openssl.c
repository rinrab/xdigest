#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stdint.h>

#include "../bench_base.h"

#define OPENSSL_SUPPRESS_DEPRECATED
#include <openssl/sha.h>
#include <openssl/md5.h>

int setup_tests(void);

/* it's lowkey the best peace of code i've ever wrote in my life */
#define ADD_ALGO(CTX, NAME) \
    do { \
        CTX *ctx; \
        xdig_bench__ctx_t *bench; \
        unsigned char digest[256]; \
        name##_Init(ctx); \
        bench = xdig_bench__ctx_create(#NAME, 1.0, 1024 * 1024); \
        while (xdig_bench__ctx_next(bench)) { \
            name##_Update(ctx, bench->buf, bench->bufsize); \
        } \
        name##_Final(ctx, digest); \
        xdig_bench__ctx_finish(bench); \
        xdig_bench__ctx_free(bench); \
        free(ctx); \
    } while (0);

int main()
{
    xdig_init();

    ADD_ALGO(SHA_CTX, SHA1);
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

