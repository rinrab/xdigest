/*
 * Copyright 1995-2023 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the Apache License 2.0 (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */

#ifndef xhash_SHA_H
# define xhash_SHA_H
# pragma once

/* ignored include 'openssl/macros.h' */
# ifndef xhash_NO_DEPRECATED_3_0
#  define HEADER_SHA_H
# endif

# include <xhash/e_os2.h>
# include <stddef.h>

# ifdef  __cplusplus
extern "C" {
# endif

# define SHA_DIGEST_LENGTH 20

# ifndef xhash_NO_DEPRECATED_3_0
/*-
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * ! SHA_LONG has to be at least 32 bits wide.                    !
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 */
#  define SHA_LONG unsigned int

#  define SHA_LBLOCK      16
#  define SHA_CBLOCK      (SHA_LBLOCK*4)/* SHA treats input data as a
                                         * contiguous array of 32 bit wide
                                         * big-endian values. */
#  define SHA_LAST_BLOCK  (SHA_CBLOCK-8)

typedef struct SHAstate_st {
    SHA_LONG h0, h1, h2, h3, h4;
    SHA_LONG Nl, Nh;
    SHA_LONG data[SHA_LBLOCK];
    unsigned int num;
} xhash_sha1_ctx_t;

int xhash_sha1_init(xhash_sha1_ctx_t *c);
int xhash_sha1_update(xhash_sha1_ctx_t *c, const void *data, size_t len);
int xhash_sha1_final(unsigned char *md, xhash_sha1_ctx_t *c);
void xhash_sha1_transform(xhash_sha1_ctx_t *c, const unsigned char *data);
# endif

unsigned char *xhash_sha1(const unsigned char *d, size_t n, unsigned char *md);

# ifndef xhash_NO_DEPRECATED_3_0
#  define SHA256_CBLOCK   (SHA_LBLOCK*4)/* SHA-256 treats input data as a
                                        * contiguous array of 32 bit wide
                                        * big-endian values. */

typedef struct SHA256state_st {
    SHA_LONG h[8];
    SHA_LONG Nl, Nh;
    SHA_LONG data[SHA_LBLOCK];
    unsigned int num, md_len;
} xhash_sha256_ctx_t;

int xhash_sha224_init(xhash_sha256_ctx_t *c);
int xhash_sha224_update(xhash_sha256_ctx_t *c,
                                        const void *data, size_t len);
int xhash_sha224_final(unsigned char *md, xhash_sha256_ctx_t *c);
int xhash_sha256_init(xhash_sha256_ctx_t *c);
int xhash_sha256_update(xhash_sha256_ctx_t *c,
                                        const void *data, size_t len);
int xhash_sha256_final(unsigned char *md, xhash_sha256_ctx_t *c);
void xhash_sha256_transform(xhash_sha256_ctx_t *c,
                                            const unsigned char *data);
# endif

unsigned char *xhash_sha224(const unsigned char *d, size_t n, unsigned char *md);
unsigned char *xhash_sha256(const unsigned char *d, size_t n, unsigned char *md);

# define SHA256_192_DIGEST_LENGTH 24
# define SHA224_DIGEST_LENGTH    28
# define SHA256_DIGEST_LENGTH    32
# define SHA384_DIGEST_LENGTH    48
# define SHA512_DIGEST_LENGTH    64

# ifndef xhash_NO_DEPRECATED_3_0
/*
 * Unlike 32-bit digest algorithms, SHA-512 *relies* on SHA_LONG64
 * being exactly 64-bit wide. See Implementation Notes in sha512.c
 * for further details.
 */
/*
 * SHA-512 treats input data as a
 * contiguous array of 64 bit
 * wide big-endian values.
 */
#  define SHA512_CBLOCK   (SHA_LBLOCK*8)
#  if (defined(_WIN32) || defined(_WIN64)) && !defined(__MINGW32__)
#   define SHA_LONG64 unsigned __int64
#  elif defined(__arch64__)
#   define SHA_LONG64 unsigned long
#  else
#   define SHA_LONG64 unsigned long long
#  endif

typedef struct SHA512state_st {
    SHA_LONG64 h[8];
    SHA_LONG64 Nl, Nh;
    union {
        SHA_LONG64 d[SHA_LBLOCK];
        unsigned char p[SHA512_CBLOCK];
    } u;
    unsigned int num, md_len;
} xhash_sha512_ctx_t;

int xhash_sha384_init(xhash_sha512_ctx_t *c);
int xhash_sha384_update(xhash_sha512_ctx_t *c,
                                        const void *data, size_t len);
int xhash_sha384_final(unsigned char *md, xhash_sha512_ctx_t *c);
int xhash_sha512_init(xhash_sha512_ctx_t *c);
int xhash_sha512_update(xhash_sha512_ctx_t *c,
                                        const void *data, size_t len);
int xhash_sha512_final(unsigned char *md, xhash_sha512_ctx_t *c);
void xhash_sha512_transform(xhash_sha512_ctx_t *c,
                                            const unsigned char *data);
# endif

unsigned char *xhash_sha384(const unsigned char *d, size_t n, unsigned char *md);
unsigned char *xhash_sha512(const unsigned char *d, size_t n, unsigned char *md);

# ifdef  __cplusplus
}
# endif

#endif
