/*
 * Copyright 1995-2025 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the Apache License 2.0 (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */

#ifndef OSSL_E_OS_H
# define OSSL_E_OS_H

# include <limits.h>
/* ignored include 'openssl/opensslconf.h' */

#include "internal/e_os2.h"
/* ignored include 'openssl/crypto.h' */

/*
 * <openssl/e_os2.h> contains what we can justify to make visible to the
 * outside; this file e_os.h is not part of the exported interface.
 */

/***********************************************/

# ifdef FUZZING_BUILD_MODE_UNSAFE_FOR_PRODUCTION
#  define xdig_memcmp memcmp
# endif

#endif
