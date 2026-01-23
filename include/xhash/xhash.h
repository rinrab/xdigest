#ifndef XHASH_XHASH_H
# define XHASH_XHASH_H 
# pragma once

# ifdef __cplusplus
extern "C" {
# endif /* __cplusplus */

/*
 * Initializes the library. This function must be called once per process.
 */
void xhash_init(void);

#define XHASH_VERSION_MAJOR 0
#define XHASH_VERSION_MINOR 1
#define XHASH_VERSION_PATCH 0

typedef struct xhash_version_t
{
    int major;
    int minor;
    int patch;
} xhash_version_t;

void
xhash_version(xhash_version_t *p_version);

# ifdef __cplusplus
}
# endif /* __cplusplus */

#endif /* ! XHASH_XHASH_H */
