/*
 * Copyright 1995-2023 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the Apache License 2.0 (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */

#ifndef xdig_SHA_H
# define xdig_SHA_H
# pragma once

/* ignored include 'openssl/macros.h' */
# ifndef xdig_NO_DEPRECATED_3_0
#  define HEADER_SHA_H
# endif

#include "internal/e_os2.h"
# include <stddef.h>

# ifdef  __cplusplus
extern "C" {
# endif

# define XDIG_SHA_DIGEST_LENGTH 20

# ifndef xdig_NO_DEPRECATED_3_0
/*-
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * ! XDIG_SHA_LONG has to be at least 32 bits wide.                    !
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 */
#  define XDIG_SHA_LONG unsigned int

#  define XDIG_SHA_LBLOCK      16
#  define XDIG_SHA_CBLOCK      (XDIG_SHA_LBLOCK*4)/* SHA treats input data as a
                                         * contiguous array of 32 bit wide
                                         * big-endian values. */
#  define XDIG_SHA_LAST_BLOCK  (XDIG_SHA_CBLOCK-8)

typedef struct SHAstate_st {
    XDIG_SHA_LONG h0, h1, h2, h3, h4;
    XDIG_SHA_LONG Nl, Nh;
    XDIG_SHA_LONG data[XDIG_SHA_LBLOCK];
    unsigned int num;
} xdig_sha1_ctx_t;

void xdig_sha1_ctx_init(xdig_sha1_ctx_t *c);
void xdig_sha1_ctx_update(xdig_sha1_ctx_t *c, const void *data, size_t len);
void xdig_sha1_ctx_final(unsigned char *md, xdig_sha1_ctx_t *c);
void xdig_sha1_ctx_transform(xdig_sha1_ctx_t *c, const unsigned char *data);
# endif

unsigned char *xdig_sha1(const unsigned char *d, size_t n, unsigned char *md);

# ifndef xdig_NO_DEPRECATED_3_0
#  define XDIG_SHA256_CBLOCK   (XDIG_SHA_LBLOCK*4)/* SHA-256 treats input data as a
                                        * contiguous array of 32 bit wide
                                        * big-endian values. */

typedef struct SHA256state_st {
    XDIG_SHA_LONG h[8];
    XDIG_SHA_LONG Nl, Nh;
    XDIG_SHA_LONG data[XDIG_SHA_LBLOCK];
    unsigned int num, md_len;
} xdig_sha256_ctx_t;

void xdig_sha224_ctx_init(xdig_sha256_ctx_t *c);
void xdig_sha224_ctx_update(xdig_sha256_ctx_t *c,
                                        const void *data, size_t len);
void xdig_sha224_ctx_final(unsigned char *md, xdig_sha256_ctx_t *c);
void xdig_sha256_ctx_init(xdig_sha256_ctx_t *c);
void xdig_sha256_ctx_update(xdig_sha256_ctx_t *c,
                                        const void *data, size_t len);
void xdig_sha256_ctx_final(unsigned char *md, xdig_sha256_ctx_t *c);
void xdig_sha256_ctx_transform(xdig_sha256_ctx_t *c,
                                            const unsigned char *data);
# endif

unsigned char *xdig_sha224(const unsigned char *d, size_t n, unsigned char *md);
unsigned char *xdig_sha256(const unsigned char *d, size_t n, unsigned char *md);

# define XDIG_SHA256_192_DIGEST_LENGTH 24
# define XDIG_SHA224_DIGEST_LENGTH    28
# define XDIG_SHA256_DIGEST_LENGTH    32
# define XDIG_SHA384_DIGEST_LENGTH    48
# define XDIG_SHA512_DIGEST_LENGTH    64

# ifndef xdig_NO_DEPRECATED_3_0
/*
 * Unlike 32-bit digest algorithms, SHA-512 *relies* on XDIG_SHA_LONG64
 * being exactly 64-bit wide. See Implementation Notes in sha512.c
 * for further details.
 */
/*
 * SHA-512 treats input data as a
 * contiguous array of 64 bit
 * wide big-endian values.
 */
#  define XDIG_SHA512_CBLOCK   (XDIG_SHA_LBLOCK*8)
#  if (defined(_WIN32) || defined(_WIN64)) && !defined(__MINGW32__)
#   define XDIG_SHA_LONG64 unsigned __int64
#  elif defined(__arch64__)
#   define XDIG_SHA_LONG64 unsigned long
#  else
#   define XDIG_SHA_LONG64 unsigned long long
#  endif

typedef struct SHA512state_st {
    XDIG_SHA_LONG64 h[8];
    XDIG_SHA_LONG64 Nl, Nh;
    union {
        XDIG_SHA_LONG64 d[XDIG_SHA_LBLOCK];
        unsigned char p[XDIG_SHA512_CBLOCK];
    } u;
    unsigned int num, md_len;
} xdig_sha512_ctx_t;

void xdig_sha384_ctx_init(xdig_sha512_ctx_t *c);
void xdig_sha384_ctx_update(xdig_sha512_ctx_t *c,
                                        const void *data, size_t len);
void xdig_sha384_ctx_final(unsigned char *md, xdig_sha512_ctx_t *c);
void xdig_sha512_ctx_init(xdig_sha512_ctx_t *c);
void xdig_sha512_ctx_update(xdig_sha512_ctx_t *c,
                                        const void *data, size_t len);
void xdig_sha512_ctx_final(unsigned char *md, xdig_sha512_ctx_t *c);
void xdig_sha512_ctx_transform(xdig_sha512_ctx_t *c,
                                            const unsigned char *data);
# endif

unsigned char *xdig_sha384(const unsigned char *d, size_t n, unsigned char *md);
unsigned char *xdig_sha512(const unsigned char *d, size_t n, unsigned char *md);

# ifdef  __cplusplus
}
# endif

#endif
