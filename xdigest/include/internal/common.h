/*
 * Copyright 1995-2025 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the Apache License 2.0 (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */

#ifndef XDIG_INTERNAL_COMMON_H
# define XDIG_INTERNAL_COMMON_H
# pragma once

# include <stdlib.h>
# include <string.h>

# if defined(__GNUC__) || defined(__clang__)
#  define xdig_likely(x)     __builtin_expect(!!(x), 1)
#  define xdig_unlikely(x)   __builtin_expect(!!(x), 0)
# else
#  define xdig_likely(x)     (x)
#  define xdig_unlikely(x)   (x)
# endif

#include <stdio.h>

void xdig_cleanse(void *ptr, size_t len);

#endif
