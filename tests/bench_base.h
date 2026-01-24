#include <assert.h>
#include <stdlib.h>
#include <stdint.h>

#include "time.h"

static void
xdig_bench__create_buffer(unsigned char *buf, size_t len)
{
    int i;
    uint32_t seed = 67;

    for (i = 0; i < len; i++)
    {
        seed = (seed * 1103515245UL + 12345UL) & 0xffffffffUL;
        buf[i] = seed & 0xff;
    }
}

typedef struct xdig_bench__ctx_t {
    const char *name;

#ifdef WIN32
    #error "not supported"
#else
    xdig_time__t start;
    xdig_time__t end;
#endif

    double requested_time;
    size_t iter;

    size_t bufsize;
    unsigned char *buf;
} xdig_bench__ctx_t;

static xdig_bench__ctx_t *
xdig_bench__ctx_create(const char *name, double requested_time, size_t bufsize)
{
    xdig_bench__ctx_t *ctx = (xdig_bench__ctx_t *)malloc(sizeof(*ctx));

    ctx->name = name;
    ctx->requested_time = requested_time;
    ctx->start = xdig_time__get();
    ctx->iter = 0;

    ctx->bufsize = bufsize;
    ctx->buf = (unsigned char *)malloc(bufsize);
    xdig_bench__create_buffer(ctx->buf, bufsize);

    return ctx;
}

static int
xdig_bench__ctx_next(xdig_bench__ctx_t *ctx)
{
    double diff;

    ctx->iter++;
    ctx->end = xdig_time__get();

    diff = xdig_time__diff(ctx->start, ctx->end);

    return (diff < ctx->requested_time);
}

static void
xdig_bench__ctx_finish(xdig_bench__ctx_t *ctx)
{
#define GB (1024 * 1024 * 1024)

    printf("%6s: Processed %.2f GB in %.2f seconds.\n",
           ctx->name,
           ((double)ctx->bufsize * ctx->iter) / GB,
           xdig_time__diff(ctx->start, ctx->end));
}

static void
xdig_bench__ctx_free(xdig_bench__ctx_t *ctx)
{
    free(ctx->buf);
    free(ctx);
}

