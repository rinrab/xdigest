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
#include <xhash/md2.h>

/*
 * This is a separate file so that #defines in cryptlib.h can map my MD
 * functions to different names
 */

unsigned char *xhash_md2(const unsigned char *d, size_t n, unsigned char *md)
{
    xhash_md2_ctx_t c;
    static unsigned char m[XHASH_MD2_DIGEST_LENGTH];

    if (md == NULL)
        md = m;
    if (!xhash_md2_init(&c))
        return NULL;
#ifndef CHARSET_EBCDIC
    xhash_md2_update(&c, d, n);
#else
    {
        char temp[1024];
        unsigned long chunk;

        while (n > 0) {
            chunk = (n > sizeof(temp)) ? sizeof(temp) : n;
            ebcdic2ascii(temp, d, chunk);
            xhash_md2_update(&c, temp, chunk);
            n -= chunk;
            d += chunk;
        }
    }
#endif
    xhash_md2_final(md, &c);
    xhash_cleanse(&c, sizeof(c)); /* Security consideration */
    return md;
}
