/*
 * Copyright 1995-2016 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the Apache License 2.0 (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */

#include <stdlib.h>
#include <string.h>
/* ignored include 'openssl/opensslconf.h' */
#include "internal/md4.h"

static void md4_block_data_order(xdig_md4_ctx_t *c, const void *p, size_t num);

#define DATA_ORDER_IS_LITTLE_ENDIAN

#define HASH_LONG               XDIG_MD4_LONG
#define HASH_CTX                xdig_md4_ctx_t
#define HASH_CBLOCK             XDIG_MD4_CBLOCK
#define HASH_UPDATE             xdig_md4_ctx_update
#define HASH_FINAL              xdig_md4_ctx_final
#define HASH_MAKE_STRING(c,s)   do {    \
        unsigned long ll;               \
        ll=(c)->A; (void)HOST_l2c(ll,(s));      \
        ll=(c)->B; (void)HOST_l2c(ll,(s));      \
        ll=(c)->C; (void)HOST_l2c(ll,(s));      \
        ll=(c)->D; (void)HOST_l2c(ll,(s));      \
        } while (0)
#define HASH_BLOCK_DATA_ORDER   md4_block_data_order

#include "crypto/md32_common.h"

/*-
#define F(x,y,z)        (((x) & (y))  |  ((~(x)) & (z)))
#define G(x,y,z)        (((x) & (y))  |  ((x) & ((z))) | ((y) & ((z))))
*/

/*
 * As pointed out by Wei Dai, the above can be simplified to the code
 * below.  Wei attributes these optimizations to Peter Gutmann's SHS code,
 * and he attributes it to Rich Schroeppel.
 */
#define F(b,c,d)        ((((c) ^ (d)) & (b)) ^ (d))
#define G(b,c,d)        (((b) & (c)) | ((b) & (d)) | ((c) & (d)))
#define H(b,c,d)        ((b) ^ (c) ^ (d))

#define R0(a,b,c,d,k,s,t) { \
        a+=((k)+(t)+F((b),(c),(d))); \
        a=ROTATE(a,s); };

#define R1(a,b,c,d,k,s,t) { \
        a+=((k)+(t)+G((b),(c),(d))); \
        a=ROTATE(a,s); };

#define R2(a,b,c,d,k,s,t) { \
        a+=((k)+(t)+H((b),(c),(d))); \
        a=ROTATE(a,s); };
