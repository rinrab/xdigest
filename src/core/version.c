#include "xhash/xhash.h"

void
xhash_version(xhash_version_t *p_version)
{
    p_version->major = XHASH_VERSION_MAJOR;
    p_version->minor = XHASH_VERSION_MINOR;
    p_version->patch = XHASH_VERSION_PATCH;
}
