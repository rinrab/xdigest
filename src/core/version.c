#include "xdigest/xdigest.h"

void
xdig_version(xdig_version_t *p_version)
{
    p_version->major = XDIG_VERSION_MAJOR;
    p_version->minor = XDIG_VERSION_MINOR;
    p_version->patch = XDIG_VERSION_PATCH;
}
