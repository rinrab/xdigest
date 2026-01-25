#ifndef XDIG_xdig_H
# define XDIG_xdig_H 
# pragma once

# ifdef __cplusplus
extern "C" {
# endif /* __cplusplus */

/*
 * Initializes the library. This function must be called once per process.
 */
void xdig_init(void);

#define XDIG_VERSION_MAJOR 0
#define XDIG_VERSION_MINOR 1
#define XDIG_VERSION_PATCH 0

typedef struct xdig_version_t
{
    int major;
    int minor;
    int patch;
} xdig_version_t;

void
xdig_version(xdig_version_t *version);

# ifdef __cplusplus
}
# endif /* __cplusplus */

#endif /* ! XDIG_xdig_H */
