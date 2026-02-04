/*
 * Copyright 1995-2025 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the Apache License 2.0 (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */

#ifndef OSSL_INTERNAL_COMMON_H
# define OSSL_INTERNAL_COMMON_H
# pragma once

# include <stdlib.h>
# include <string.h>
/* ignored include 'openssl/configuration.h' */

# include "internal/e_os.h" /* ossl_inline in many files */
/* ignored include 'internal/nelem.h' */

# if defined(__GNUC__) || defined(__clang__)
#  define ossl_likely(x)     __builtin_expect(!!(x), 1)
#  define ossl_unlikely(x)   __builtin_expect(!!(x), 0)
# else
#  define ossl_likely(x)     (x)
#  define ossl_unlikely(x)   (x)
# endif

#include <stdio.h>

void xdig_cleanse(void *ptr, size_t len);

static ossl_inline int ossl_ends_with_dirsep(const char *path)
{
    if (*path != '\0')
        path += strlen(path) - 1;
# if defined __VMS
    if (*path == ']' || *path == '>' || *path == ':')
        return 1;
# elif defined _WIN32
    if (*path == '\\')
        return 1;
# endif
    return *path == '/';
}

static ossl_inline char ossl_determine_dirsep(const char *path)
{
    if (ossl_ends_with_dirsep(path))
        return '\0';

# if defined(_WIN32)
    return '\\';
# elif defined(__VMS)
    return ':';
# else
    return '/';
# endif
}

static ossl_inline int ossl_is_absolute_path(const char *path)
{
# if defined __VMS
    if (strchr(path, ':') != NULL
        || ((path[0] == '[' || path[0] == '<')
            && path[1] != '.' && path[1] != '-'
            && path[1] != ']' && path[1] != '>'))
        return 1;
# elif defined _WIN32
    if (path[0] == '\\'
        || (path[0] != '\0' && path[1] == ':'))
        return 1;
# endif
    return path[0] == '/';
}

const char *ossl_get_openssldir(void);
const char *ossl_get_enginesdir(void);
const char *ossl_get_modulesdir(void);
const char *ossl_get_wininstallcontext(void);

#endif
