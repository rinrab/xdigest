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
#include "xdigest_md2.h"

# ifndef xdig_NO_MD2
#  include <stddef.h>
#  ifdef  __cplusplus
extern "C" {
#  endif

#  define XDIG_MD2_DIGEST_LENGTH       16

#  if !defined(xdig_NO_DEPRECATED_3_0)

typedef unsigned char MD2_INT;

#   define MD2_BLOCK               16

struct xdig_md2_ctx_t {
    unsigned int num;
    unsigned char data[MD2_BLOCK];
    MD2_INT cksm[MD2_BLOCK];
    MD2_INT state[MD2_BLOCK];
};
#  endif
#  ifndef xdig_NO_DEPRECATED_3_0

#  endif

#  ifdef  __cplusplus
}
#  endif
# endif
#endif
