/*
 * sha1.c: public interface for SHA1 algorithm
 *
 * ====================================================================
 * Copyright 2026, Timofei Zhakov
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * ====================================================================
 */

#include "internal/sha.h"

#include "sha_local.h"

unsigned char *xdig_sha1(unsigned char *digest, const void *data, size_t len)
{
    xdig_sha1_ctx_t ctx;

    xdig_sha1_ctx_init(&ctx);
    xdig_sha1_ctx_update(&ctx, data, len);
    xdig_sha1_ctx_final(&ctx, digest);

    return digest;
}

size_t xdig_sha1_ctx_size(void)
{
    return sizeof(xdig_sha1_ctx_t);
}
