/*
 * Copyright 1995-2020 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the Apache License 2.0 (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */

/*
 * MD2 low level APIs are deprecated for public use, but still ok for
 * internal use.
 */
/* ignored include 'internal/deprecated.h' */

#include <stdio.h>
/* ignored include 'internal/cryptlib.h' */
#include "internal/md2.h"
#include "internal/common.h"

/*
 * This is a separate file so that #defines in cryptlib.h can map my MD
 * functions to different names
 */

unsigned char *
xdig_md2(const unsigned char *data, size_t len, unsigned char *digest)
{
    xdig_md2_ctx_t ctx;

    xdig_md2_ctx_init(&ctx);
    xdig_md2_ctx_update(&ctx, data, len);
    xdig_md2_ctx_final(&ctx, digest);
    xdig_cleanse(&ctx, sizeof(ctx)); /* Security consideration */

    return digest;
}

size_t xdig_md2_ctx_size(void)
{
    return sizeof(xdig_md2_ctx_t);
}
