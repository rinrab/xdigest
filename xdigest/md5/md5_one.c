/*
 * Copyright 1995-2020 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the Apache License 2.0 (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */

/*
 * MD5 low level APIs are deprecated for public use, but still ok for
 * internal use.
 */
/* ignored include 'internal/deprecated.h' */

#include <stdio.h>
#include <string.h>
#include "internal/md5.h"
/* ignored include 'openssl/crypto.h' */
#include "internal/common.h"

#ifdef CHARSET_EBCDIC
/* ignored include 'openssl/ebcdic.h' */
#endif

unsigned char *
xdig_md5(const unsigned char *data, size_t len, unsigned char *digest)
{
    xdig_md5_ctx_t ctx;

    xdig_md5_ctx_init(&ctx);
    xdig_md5_ctx_update(&ctx, data, len);
    xdig_md5_ctx_final(digest, &ctx);
    xdig_cleanse(&ctx, sizeof(ctx)); /* security consideration */

    return digest;
}

size_t xdig_md5_ctx_size(void)
{
    return sizeof(xdig_md5_ctx_t);
}
