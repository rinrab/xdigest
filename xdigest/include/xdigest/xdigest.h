/*
 * @copyright
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
 * @endcopyright
 *
 * @file xdigest.h
 * @brief xdigest core methods
 */

#ifndef XDIG_xdig_H
# define XDIG_xdig_H 
# pragma once

# ifdef __cplusplus
extern "C" {
# endif /* __cplusplus */

/*
 * Initializes the library. This function must be called once per process.
 */
void xdig_init(void);

#define XDIG_VERSION_MAJOR 0
#define XDIG_VERSION_MINOR 2
#define XDIG_VERSION_PATCH 0

typedef struct xdig_version_t
{
    int major;
    int minor;
    int patch;
} xdig_version_t;

void
xdig_version(xdig_version_t *version);

# ifdef __cplusplus
}
# endif /* __cplusplus */

#endif /* ! XDIG_xdig_H */
