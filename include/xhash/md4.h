/*
 * Copyright 1995-2020 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the Apache License 2.0 (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */

#ifndef xhash_MD4_H
# define xhash_MD4_H
# pragma once

/* ignored include 'openssl/macros.h' */
# ifndef xhash_NO_DEPRECATED_3_0
#  define HEADER_MD4_H
# endif

/* ignored include 'openssl/opensslconf.h' */

# ifndef xhash_NO_MD4
#include "internal/e_os2.h"
#  include <stddef.h>
#  ifdef  __cplusplus
extern "C" {
#   endif

#  define XHASH_MD4_DIGEST_LENGTH 16

#  if !defined(xhash_NO_DEPRECATED_3_0)

/*-
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * ! XHASH_MD4_LONG has to be at least 32 bits wide.                     !
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 */
#   define XHASH_MD4_LONG unsigned int

#   define XHASH_MD4_CBLOCK      64
#   define XHASH_MD4_LBLOCK      (XHASH_MD4_CBLOCK/4)

typedef struct MD4state_st {
    XHASH_MD4_LONG A, B, C, D;
    XHASH_MD4_LONG Nl, Nh;
    XHASH_MD4_LONG data[XHASH_MD4_LBLOCK];
    unsigned int num;
} xhash_md4_ctx_t;
#  endif
#  ifndef xhash_NO_DEPRECATED_3_0
int xhash_md4_init(xhash_md4_ctx_t *c);
int xhash_md4_update(xhash_md4_ctx_t *c, const void *data, size_t len);
int xhash_md4_final(unsigned char *md, xhash_md4_ctx_t *c);
unsigned char *xhash_md4(const unsigned char *d, size_t n,
                                         unsigned char *md);
void xhash_md4_transform(xhash_md4_ctx_t *c, const unsigned char *b);
#  endif

#  ifdef  __cplusplus
}
#  endif
# endif

#endif
