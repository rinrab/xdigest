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

#include "internal/e_os2.h"
#include "xdigest/xdigest_sha1.h"
#include "xdigest/xdigest_sha2.h"
#include <stddef.h>

# ifdef  __cplusplus
extern "C" {
# endif

# define XDIG_SHA_DIGEST_LENGTH 20

/*-
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * ! XDIG_SHA_LONG has to be at least 32 bits wide.               !
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 */
# define XDIG_SHA_LONG unsigned int

/*
 * SHA treats input data as a contiguous array of 32 bit wide big-endian
 * values.
 */
# define XDIG_SHA_LBLOCK      16
# define XDIG_SHA_CBLOCK      (XDIG_SHA_LBLOCK*4)
# define XDIG_SHA_LAST_BLOCK  (XDIG_SHA_CBLOCK-8)

struct xdig_sha1_ctx_t {
    XDIG_SHA_LONG h0, h1, h2, h3, h4;
    XDIG_SHA_LONG Nl, Nh;
    XDIG_SHA_LONG data[XDIG_SHA_LBLOCK];
    unsigned int num;
};

/*
 * SHA-256 treats input data as a contiguous array of 32 bit wide big-endian
 * values.
 */
# define XDIG_SHA256_CBLOCK   (XDIG_SHA_LBLOCK*4)

struct xdig_sha256_ctx_t {
    XDIG_SHA_LONG h[8];
    XDIG_SHA_LONG Nl, Nh;
    XDIG_SHA_LONG data[XDIG_SHA_LBLOCK];
    unsigned int num, md_len;
};

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
# define XDIG_SHA512_CBLOCK   (XDIG_SHA_LBLOCK*8)
# if (defined(_WIN32) || defined(_WIN64)) && !defined(__MINGW32__)
#  define XDIG_SHA_LONG64 unsigned __int64
# elif defined(__arch64__)
#  define XDIG_SHA_LONG64 unsigned long
# else
#  define XDIG_SHA_LONG64 unsigned long long
# endif

struct xdig_sha512_ctx_t {
    XDIG_SHA_LONG64 h[8];
    XDIG_SHA_LONG64 Nl, Nh;
    union {
        XDIG_SHA_LONG64 d[XDIG_SHA_LBLOCK];
        unsigned char p[XDIG_SHA512_CBLOCK];
    } u;
    unsigned int num, md_len;
};

# ifdef  __cplusplus
}
# endif

#endif /* ! xdig_SHA_H */
