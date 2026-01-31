/*
 * Copyright 1995-2020 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the Apache License 2.0 (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */

#ifndef xdig_MD4_H
# define xdig_MD4_H
# pragma once

/* ignored include 'openssl/macros.h' */
# ifndef xdig_NO_DEPRECATED_3_0
#  define HEADER_MD4_H
# endif

/* ignored include 'openssl/opensslconf.h' */

# ifndef xdig_NO_MD4
#include "internal/e_os2.h"
#  include <stddef.h>
#  ifdef  __cplusplus
extern "C" {
#   endif

#  define XDIG_MD4_DIGEST_LENGTH 16

#  if !defined(xdig_NO_DEPRECATED_3_0)

/*-
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * ! XDIG_MD4_LONG has to be at least 32 bits wide.                     !
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 */
#   define XDIG_MD4_LONG unsigned int

#   define XDIG_MD4_CBLOCK      64
#   define XDIG_MD4_LBLOCK      (XDIG_MD4_CBLOCK/4)

typedef struct MD4state_st {
    XDIG_MD4_LONG A, B, C, D;
    XDIG_MD4_LONG Nl, Nh;
    XDIG_MD4_LONG data[XDIG_MD4_LBLOCK];
    unsigned int num;
} xdig_md4_ctx_t;
#  endif
#  ifndef xdig_NO_DEPRECATED_3_0
void xdig_md4_ctx_init(xdig_md4_ctx_t *c);
void xdig_md4_ctx_update(xdig_md4_ctx_t *c, const void *data, size_t len);
void xdig_md4_ctx_final(xdig_md4_ctx_t *c, unsigned char *md);
unsigned char *xdig_md4(const unsigned char *d, size_t n,
                                         unsigned char *md);
#  endif

#  ifdef  __cplusplus
}
#  endif
# endif

#endif
