/*
 * Copyright 2002-2016 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the Apache License 2.0 (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */

#ifdef NO_ASM

#include <string.h>
/* ignored include 'openssl/crypto.h' */

void xhash_cleanse(void *ptr, size_t len)
{
    memset(ptr, 0, len);
}

#endif /* NO_ASM */
