#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#if WIN32
# error "TODO"
#else
#include <time.h>

#define BILLION  1000000000L;

typedef struct timespec xhash_time__t;

static xhash_time__t xhash_time__get()
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

static double
xhash_time__diff(xhash_time__t start, xhash_time__t end)
{
    return (end.tv_sec - start.tv_sec)
             + (double)(end.tv_nsec - start.tv_nsec)
               / (double)BILLION;
}
#endif

