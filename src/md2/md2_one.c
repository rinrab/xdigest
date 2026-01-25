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

unsigned char *xdig_md2(const unsigned char *d, size_t n, unsigned char *md)
{
    xdig_md2_ctx_t c;
    static unsigned char m[XDIG_MD2_DIGEST_LENGTH];

    if (md == NULL)
        md = m;
    if (!xdig_md2_init(&c))
        return NULL;
#ifndef CHARSET_EBCDIC
    xdig_md2_update(&c, d, n);
#else
    {
        char temp[1024];
        unsigned long chunk;

        while (n > 0) {
            chunk = (n > sizeof(temp)) ? sizeof(temp) : n;
            ebcdic2ascii(temp, d, chunk);
            xdig_md2_update(&c, temp, chunk);
            n -= chunk;
            d += chunk;
        }
    }
#endif
    xdig_md2_final(md, &c);
    xdig_cleanse(&c, sizeof(c)); /* Security consideration */
    return md;
}

size_t xdig_md2_ctx_size(void)
{
    return sizeof(xdig_md2_ctx_t);
}
