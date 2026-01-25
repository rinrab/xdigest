/*
 * Copyright 2018-2023 The OpenSSL Project Authors. All Rights Reserved.
 * Copyright (c) 2018, Oracle and/or its affiliates.  All rights reserved.
 *
 * Licensed under the Apache License 2.0 (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */

#ifndef OSSL_xdig_SHA_H
# define OSSL_xdig_SHA_H
# pragma once

#include "internal/sha.h"

int ossl_sha256_192_init(xdig_sha256_ctx_t *c);
int sha512_224_init(xdig_sha512_ctx_t *);
int sha512_256_init(xdig_sha512_ctx_t *);
int ossl_sha1_ctrl(xdig_sha1_ctx_t *ctx, int cmd, int mslen, void *ms);
unsigned char *ossl_sha1(const unsigned char *d, size_t n, unsigned char *md);

#endif
