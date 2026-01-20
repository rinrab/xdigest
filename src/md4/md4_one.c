/*
 * Copyright 1995-2020 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the Apache License 2.0 (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */

/*
 * MD4 low level APIs are deprecated for public use, but still ok for
 * internal use.
 */
/* ignored include 'internal/deprecated.h' */

#include <stdio.h>
#include <string.h>
#include <xhash/md4.h>
/* ignored include 'openssl/crypto.h' */

#ifdef CHARSET_EBCDIC
# include <xhash/ebcdic.h>
#endif

unsigned char *xhash_md4(const unsigned char *d, size_t n, unsigned char *md)
{
    xhash_md4_ctx_t c;
    static unsigned char m[XHASH_MD4_DIGEST_LENGTH];

    if (md == NULL)
        md = m;
    if (!xhash_md4_init(&c))
        return NULL;
#ifndef CHARSET_EBCDIC
    xhash_md4_update(&c, d, n);
#else
    {
        char temp[1024];
        unsigned long chunk;

        while (n > 0) {
            chunk = (n > sizeof(temp)) ? sizeof(temp) : n;
            ebcdic2ascii(temp, d, chunk);
            xhash_md4_update(&c, temp, chunk);
            n -= chunk;
            d += chunk;
        }
    }
#endif
    xhash_md4_final(md, &c);
    xhash_cleanse(&c, sizeof(c)); /* security consideration */
    return md;
}
