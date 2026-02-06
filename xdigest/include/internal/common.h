/*
 * internal/common.h: platform dependent common things
 *
 * ====================================================================
 * Copyright 1995-2025 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the Apache License 2.0 (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 * ====================================================================
 * Copyright 2026, Timofei Zhakov
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * ====================================================================
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
