/*
 * Copyright 1995-2020 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the Apache License 2.0 (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */

#ifndef xhash_MD5_H
# define xhash_MD5_H
# pragma once

# ifndef xhash_NO_MD5
#include "internal/e_os2.h"
#  include <stddef.h>
#  ifdef  __cplusplus
extern "C" {
#  endif

#  define XHASH_MD5_DIGEST_LENGTH 16

/*
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * ! XHASH_MD5_LONG has to be at least 32 bits wide.                     !
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 */
#   define XHASH_MD5_LONG unsigned int

#   define XHASH_MD5_CBLOCK      64
#   define XHASH_MD5_LBLOCK      (XHASH_MD5_CBLOCK/4)

typedef struct MD5state_st {
    XHASH_MD5_LONG A, B, C, D;
    XHASH_MD5_LONG Nl, Nh;
    XHASH_MD5_LONG data[XHASH_MD5_LBLOCK];
    unsigned int num;
} xhash_md5_ctx_t;

int
xhash_md5_init(xhash_md5_ctx_t *c);

int
xhash_md5_update(xhash_md5_ctx_t *c,
                 const void *data,
                 size_t len);

int
xhash_md5_final(unsigned char *md,
                xhash_md5_ctx_t *c);

unsigned char *
xhash_md5(const unsigned char *d,
          size_t n,
          unsigned char *md);

void
xhash_md5_transform(xhash_md5_ctx_t *c,
                    const unsigned char *b);

#  ifdef  __cplusplus
}
#  endif
# endif

#endif
