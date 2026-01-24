#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#define BILLION 1000000000L

#if __WIN32__ 
#include <windows.h>

typedef LARGE_INTEGER xdig_time__t;

static xdig_time__t xdig_time__get()
{
    LARGE_INTEGER result = { 0 };

    if (! QueryPerformanceCounter())
       abort();

    return result;
}

static double
xdig_time__diff(xdig_time__t start, xdig_time__t end)
{
    LONG_INTEGER frequency = { 0 };

    if (! QueryPerformanceFrequency(&frequency))
       abort();

    return (double)(end.QuadPart - start.QuadPart) / frequency.QuadPart;
}
#else
#include <time.h>

typedef struct timespec xdig_time__t;

static xdig_time__t xdig_time__get()
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
xdig_time__diff(xdig_time__t start, xdig_time__t end)
{
    return (end.tv_sec - start.tv_sec)
             + (double)(end.tv_nsec - start.tv_nsec)
               / (double)BILLION;
}
#endif

