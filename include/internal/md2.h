/*
 * Copyright 1995-2020 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the Apache License 2.0 (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */

#ifndef xhash_MD2_H
# define xhash_MD2_H
# pragma once

/* ignored include 'openssl/macros.h' */
# ifndef xhash_NO_DEPRECATED_3_0
#  define HEADER_MD2_H
# endif

/* ignored include 'openssl/opensslconf.h' */

# ifndef xhash_NO_MD2
#  include <stddef.h>
#  ifdef  __cplusplus
extern "C" {
#  endif

#  define XHASH_MD2_DIGEST_LENGTH       16

#  if !defined(xhash_NO_DEPRECATED_3_0)

typedef unsigned char MD2_INT;

#   define MD2_BLOCK               16

typedef struct MD2state_st {
    unsigned int num;
    unsigned char data[MD2_BLOCK];
    MD2_INT cksm[MD2_BLOCK];
    MD2_INT state[MD2_BLOCK];
} xhash_md2_ctx_t;
#  endif
#  ifndef xhash_NO_DEPRECATED_3_0
const char *xhash_md2_options(void);
int xhash_md2_init(xhash_md2_ctx_t *c);
int xhash_md2_update(xhash_md2_ctx_t *c, const unsigned char *data,
                                     size_t len);
int xhash_md2_final(unsigned char *md, xhash_md2_ctx_t *c);
unsigned char *xhash_md2(const unsigned char *d, size_t n,
                                         unsigned char *md);
#  endif

#  ifdef  __cplusplus
}
#  endif
# endif
#endif
