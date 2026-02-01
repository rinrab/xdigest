/*
 * Copyright 2021 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the Apache License 2.0 (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */

#include <string.h>
#include <assert.h>
#include <stdio.h>

#ifdef XDIG
#include <xdigest_sha1.h>
#include <xdigest_sha2.h>
#else
#include <openssl/sha.h>

#define xdig_sha1 SHA1
#define xdig_sha224 SHA224
#define xdig_sha256 SHA256
#define xdig_sha384 SHA384
#define xdig_sha512 SHA512
#endif

#define TEST_ASSERT(expr) do { \
    int __r = (expr); \
    if (!__r) { \
        printf("test assertion failed: %s\n", #expr); \
        return 0; \
    } } while (0);

static int test_static_sha1(void)
{
    unsigned char expected[XDIG_SHA1_DIGEST_LENGTH] = {
        0xa9, 0x99, 0x3e, 0x36, 0x47, 0x06, 0x81, 0x6a,
        0xba, 0x3e, 0x25, 0x71, 0x78, 0x50, 0xc2, 0x6c,
        0x9c, 0xd0, 0xd8, 0x9d
    };
    unsigned char actual[sizeof(expected)];

    xdig_sha1(actual, (unsigned char *)"abc", 3);
    TEST_ASSERT(memcmp(expected, actual, sizeof(expected)) == 0);

    return 1;
}

static int test_static_sha224(void)
{
    unsigned char expected[XDIG_SHA224_DIGEST_LENGTH] = {
        0x23, 0x09, 0x7d, 0x22, 0x34, 0x05, 0xd8, 0x22,
        0x86, 0x42, 0xa4, 0x77, 0xbd, 0xa2, 0x55, 0xb3,
        0x2a, 0xad, 0xbc, 0xe4, 0xbd, 0xa0, 0xb3, 0xf7,
        0xe3, 0x6c, 0x9d, 0xa7
    };
    unsigned char actual[sizeof(expected)];

    xdig_sha224(actual, (unsigned char *)"abc", 3);
    TEST_ASSERT(memcmp(expected, actual, sizeof(expected)) == 0);

    return 1;
}

static int test_static_sha256(void)
{
    unsigned char expected[XDIG_SHA256_DIGEST_LENGTH] = {
        0xba, 0x78, 0x16, 0xbf, 0x8f, 0x01, 0xcf, 0xea,
        0x41, 0x41, 0x40, 0xde, 0x5d, 0xae, 0x22, 0x23,
        0xb0, 0x03, 0x61, 0xa3, 0x96, 0x17, 0x7a, 0x9c,
        0xb4, 0x10, 0xff, 0x61, 0xf2, 0x00, 0x15, 0xad
    };
    unsigned char actual[sizeof(expected)];

    xdig_sha256(actual, (unsigned char *)"abc", 3);
    TEST_ASSERT(memcmp(expected, actual, sizeof(expected)) == 0);

    return 1;
}

static int test_static_sha384(void)
{
    unsigned char expected[XDIG_SHA384_DIGEST_LENGTH] = {
        0xcb, 0x00, 0x75, 0x3f, 0x45, 0xa3, 0x5e, 0x8b,
        0xb5, 0xa0, 0x3d, 0x69, 0x9a, 0xc6, 0x50, 0x07,
        0x27, 0x2c, 0x32, 0xab, 0x0e, 0xde, 0xd1, 0x63,
        0x1a, 0x8b, 0x60, 0x5a, 0x43, 0xff, 0x5b, 0xed,
        0x80, 0x86, 0x07, 0x2b, 0xa1, 0xe7, 0xcc, 0x23,
        0x58, 0xba, 0xec, 0xa1, 0x34, 0xc8, 0x25, 0xa7
    };
    unsigned char actual[sizeof(expected)];

    xdig_sha384(actual, (unsigned char *)"abc", 3);
    TEST_ASSERT(memcmp(expected, actual, sizeof(expected)) == 0);

    return 1;
}

static int test_static_sha512(void)
{
    unsigned char expected[XDIG_SHA512_DIGEST_LENGTH] = {
        0xdd, 0xaf, 0x35, 0xa1, 0x93, 0x61, 0x7a, 0xba,
        0xcc, 0x41, 0x73, 0x49, 0xae, 0x20, 0x41, 0x31,
        0x12, 0xe6, 0xfa, 0x4e, 0x89, 0xa9, 0x7e, 0xa2,
        0x0a, 0x9e, 0xee, 0xe6, 0x4b, 0x55, 0xd3, 0x9a,
        0x21, 0x92, 0x99, 0x2a, 0x27, 0x4f, 0xc1, 0xa8,
        0x36, 0xba, 0x3c, 0x23, 0xa3, 0xfe, 0xeb, 0xbd,
        0x45, 0x4d, 0x44, 0x23, 0x64, 0x3c, 0xe8, 0x0e,
        0x2a, 0x9a, 0xc9, 0x4f, 0xa5, 0x4c, 0xa4, 0x9f
    };

    unsigned char actual[sizeof(expected)];

    xdig_sha512(actual, (unsigned char *)"abc", 3);
    TEST_ASSERT(memcmp(expected, actual, sizeof(expected)) == 0);

    return 1;
}

#define ADD_TEST(func) do { \
    if (func()) { \
        printf("PASS %s\n", #func); \
    } else { \
        printf("FAIL %s\n", #func); \
    } \
} while (0);

int setup_tests(void)
{
    ADD_TEST(test_static_sha1);
    ADD_TEST(test_static_sha224);
    ADD_TEST(test_static_sha256);
    ADD_TEST(test_static_sha384);
    ADD_TEST(test_static_sha512);
    return 1;
}
