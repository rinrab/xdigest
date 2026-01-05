#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include <openssl/sha.h>

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

int main()
{
    SHA_CTX ctx = { 0 };

    unsigned char digest[SHA_DIGEST_LENGTH];
    unsigned char *buf;

    time__t start, end;
    size_t iter = 0, i;
    uint32_t seed = 67;

#ifdef XHASH
    xhash_cpuid_setup();
#endif

    buf = malloc(BUFSIZE);
    for (i = 0; i < BUFSIZE; i++)
    {
        seed = (seed * 1103515245UL + 12345UL) & 0xffffffffUL;
        buf[i] = seed & 0xff;
    }

    start = time__get();
    SHA1_Init(&ctx);

    do
    {
        SHA1_Update(&ctx, buf, BUFSIZE);
        end = time__get();
        iter++;
    }
    while (time__diff(start, end) < 1.0);

    SHA1_Final(digest, &ctx);

    printf("Processed %.2f GB in %.2f seconds.\n",
           ((double)BUFSIZE * iter) / (1024 * 1024 * 1024),
           time__diff(start, end));

    free(buf);
    return 0;
}

