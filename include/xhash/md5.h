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

/* ignored include 'openssl/macros.h' */
# ifndef xhash_NO_DEPRECATED_3_0
#  define HEADER_MD5_H
# endif

/* ignored include 'openssl/opensslconf.h' */

# ifndef xhash_NO_MD5
#  include <xhash/e_os2.h>
#  include <stddef.h>
#  ifdef  __cplusplus
extern "C" {
#  endif

#  define MD5_DIGEST_LENGTH 16

#  if !defined(xhash_NO_DEPRECATED_3_0)
/*
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * ! MD5_LONG has to be at least 32 bits wide.                     !
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 */
#   define MD5_LONG unsigned int

#   define MD5_CBLOCK      64
#   define MD5_LBLOCK      (MD5_CBLOCK/4)

typedef struct MD5state_st {
    MD5_LONG A, B, C, D;
    MD5_LONG Nl, Nh;
    MD5_LONG data[MD5_LBLOCK];
    unsigned int num;
} xhash_md5_ctx_t;
#  endif
#  ifndef xhash_NO_DEPRECATED_3_0
OSSL_DEPRECATEDIN_3_0 int MD5_Init(xhash_md5_ctx_t *c);
OSSL_DEPRECATEDIN_3_0 int MD5_Update(xhash_md5_ctx_t *c, const void *data, size_t len);
OSSL_DEPRECATEDIN_3_0 int MD5_Final(unsigned char *md, xhash_md5_ctx_t *c);
OSSL_DEPRECATEDIN_3_0 unsigned char *MD5(const unsigned char *d, size_t n,
                                         unsigned char *md);
OSSL_DEPRECATEDIN_3_0 void MD5_Transform(xhash_md5_ctx_t *c, const unsigned char *b);
#  endif

#  ifdef  __cplusplus
}
#  endif
# endif

#endif
