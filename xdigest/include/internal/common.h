/*
 * Copyright 1995-2025 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the Apache License 2.0 (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */

#ifndef OSSL_INTERNAL_COMMON_H
# define OSSL_INTERNAL_COMMON_H
# pragma once

# include <stdlib.h>
# include <string.h>
/* ignored include 'openssl/configuration.h' */

# include "internal/e_os.h" /* ossl_inline in many files */
/* ignored include 'internal/nelem.h' */

# if defined(__GNUC__) || defined(__clang__)
#  define ossl_likely(x)     __builtin_expect(!!(x), 1)
#  define ossl_unlikely(x)   __builtin_expect(!!(x), 0)
# else
#  define ossl_likely(x)     (x)
#  define ossl_unlikely(x)   (x)
# endif

#include <stdio.h>

void xdig_cleanse(void *ptr, size_t len);

#endif
