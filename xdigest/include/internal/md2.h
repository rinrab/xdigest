/*
 * Copyright 1995-2020 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the Apache License 2.0 (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */

#ifndef xdig_MD2_H
# define xdig_MD2_H
# pragma once

/* ignored include 'openssl/macros.h' */
# ifndef xdig_NO_DEPRECATED_3_0
#  define HEADER_MD2_H
# endif

/* ignored include 'openssl/opensslconf.h' */

# ifndef xdig_NO_MD2
#  include <stddef.h>
#  ifdef  __cplusplus
extern "C" {
#  endif

#  define XDIG_MD2_DIGEST_LENGTH       16

#  if !defined(xdig_NO_DEPRECATED_3_0)

typedef unsigned char MD2_INT;

#   define MD2_BLOCK               16

typedef struct MD2state_st {
    unsigned int num;
    unsigned char data[MD2_BLOCK];
    MD2_INT cksm[MD2_BLOCK];
    MD2_INT state[MD2_BLOCK];
} xdig_md2_ctx_t;
#  endif
#  ifndef xdig_NO_DEPRECATED_3_0
const char *xdig_md2_ctx_options(void);
void xdig_md2_ctx_init(xdig_md2_ctx_t *c);
void xdig_md2_ctx_update(xdig_md2_ctx_t *c, const unsigned char *data,
                                     size_t len);
void xdig_md2_ctx_final(xdig_md2_ctx_t *c, unsigned char *md);
unsigned char *xdig_md2(const unsigned char *d, size_t n,
                                         unsigned char *md);
#  endif

#  ifdef  __cplusplus
}
#  endif
# endif
#endif
