/*
 * Copyright 1995-2020 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the Apache License 2.0 (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */

#ifndef xdig_MD5_H
# define xdig_MD5_H
# pragma once

/* ignored include 'openssl/macros.h' */
# ifndef xdig_NO_DEPRECATED_3_0
#  define HEADER_MD5_H
# endif

/* ignored include 'openssl/opensslconf.h' */

# ifndef xdig_NO_MD5
#include "internal/e_os2.h"
#include "xdigest_md5.h"
#  include <stddef.h>
#  ifdef  __cplusplus
extern "C" {
#  endif

#  define XDIG_MD5_DIGEST_LENGTH 16

#  if !defined(xdig_NO_DEPRECATED_3_0)
/*
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * ! XDIG_MD5_LONG has to be at least 32 bits wide.                     !
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 */
#   define XDIG_MD5_LONG unsigned int

#   define XDIG_MD5_CBLOCK      64
#   define XDIG_MD5_LBLOCK      (XDIG_MD5_CBLOCK/4)

struct xdig_md5_ctx_t {
    XDIG_MD5_LONG A, B, C, D;
    XDIG_MD5_LONG Nl, Nh;
    XDIG_MD5_LONG data[XDIG_MD5_LBLOCK];
    unsigned int num;
};

#  endif

#  ifdef  __cplusplus
}
#  endif
# endif

#endif
