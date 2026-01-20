#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#ifdef XHASH
#include <xhash/sha.h>
#include <xhash/core.h>
#else
#include <openssl/sha.h>

#define xhash_sha1_ctx_t SHA_CTX
#define xhash_sha1_init SHA1_Init
#define xhash_sha1_update SHA1_Update
#define xhash_sha1_final SHA1_Final
#endif

#define BUFSIZE 1024 * 1024

#if WIN32
# error "TODO"
#else
#include <time.h>

#define BILLION  1000000000L;

typedef struct timespec time__t;

time__t time__get()
{
    struct timespec time;

    int status = clock_gettime(CLOCK_REALTIME, &time);

    if (status == -1)
    {
        perror("clock_gettime");
        abort();
    }

    return time;
}

double time__diff(time__t start, time__t end)
{
    return (end.tv_sec - start.tv_sec)
             + (double)(end.tv_nsec - start.tv_nsec)
               / (double)BILLION;
}
#endif

int setup_tests(void);

int main()
{
    xhash_sha1_ctx_t ctx = { 0 };

    unsigned char digest[XHASH_SHA_DIGEST_LENGTH];
    unsigned char *buf;

    time__t start, end;
    size_t iter = 0, i;
    uint32_t seed = 67;

#ifdef XHASH
    xhash_init();
#endif

    buf = malloc(BUFSIZE);
    for (i = 0; i < BUFSIZE; i++)
    {
        seed = (seed * 1103515245UL + 12345UL) & 0xffffffffUL;
        buf[i] = seed & 0xff;
    }

    start = time__get();
    xhash_sha1_init(&ctx);

    do
    {
        xhash_sha1_update(&ctx, buf, BUFSIZE);
        end = time__get();
        iter++;
    }
    while (time__diff(start, end) < 1.0);

    xhash_sha1_final(digest, &ctx);

    printf("Processed %.2f GB in %.2f seconds.\n",
           ((double)BUFSIZE * iter) / (1024 * 1024 * 1024),
           time__diff(start, end));

    free(buf);

    setup_tests();

    return 0;
}

