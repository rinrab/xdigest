/*
 * internal/e_os2.h: platform dependent things
 *
 * ====================================================================
 * Copyright 1995-2024 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the Apache License 2.0 (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
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

#ifndef xdig_E_OS2_H
# define xdig_E_OS2_H
# pragma once

#ifdef  __cplusplus
extern "C" {
#endif

/******************************************************************************
 * Detect operating systems.  This probably needs completing.
 * The result is that at least one xdig_SYS_os macro should be defined.
 * However, if none is defined, Unix is assumed.
 **/

# define xdig_SYS_UNIX

/* --------------------- Microsoft operating systems ---------------------- */

/*
 * Note that MSDOS actually denotes 32-bit environments running on top of
 * MS-DOS, such as DJGPP one.
 */
# if defined(xdig_SYS_MSDOS)
#  undef xdig_SYS_UNIX
# endif

/*
 * For 32 bit environment, there seems to be the CygWin environment and then
 * all the others that try to do the same thing Microsoft does...
 */
/*
 * UEFI lives here because it might be built with a Microsoft toolchain and
 * we need to avoid the false positive match on Windows.
 */
# if defined(xdig_SYS_UEFI)
#  undef xdig_SYS_UNIX
# elif defined(xdig_SYS_UWIN)
#  undef xdig_SYS_UNIX
#  define xdig_SYS_WIN32_UWIN
# else
#  if defined(__CYGWIN__) || defined(xdig_SYS_CYGWIN)
#   define xdig_SYS_WIN32_CYGWIN
#  else
#   if defined(_WIN32) || defined(xdig_SYS_WIN32)
#    undef xdig_SYS_UNIX
#    if !defined(xdig_SYS_WIN32)
#     define xdig_SYS_WIN32
#    endif
#   endif
#   if defined(_WIN64) || defined(xdig_SYS_WIN64)
#    undef xdig_SYS_UNIX
#    if !defined(xdig_SYS_WIN64)
#     define xdig_SYS_WIN64
#    endif
#   endif
#   if defined(xdig_SYS_WINNT)
#    undef xdig_SYS_UNIX
#   endif
#   if defined(xdig_SYS_WINCE)
#    undef xdig_SYS_UNIX
#   endif
#  endif
# endif

/* Anything that tries to look like Microsoft is "Windows" */
# if defined(xdig_SYS_WIN32) || defined(xdig_SYS_WIN64) || defined(xdig_SYS_WINNT) || defined(xdig_SYS_WINCE)
#  undef xdig_SYS_UNIX
#  define xdig_SYS_WINDOWS
#  ifndef xdig_SYS_MSDOS
#   define xdig_SYS_MSDOS
#  endif
# endif

/*
 * DLL settings.  This part is a bit tough, because it's up to the
 * application implementer how he or she will link the application, so it
 * requires some macro to be used.
 */
# ifdef xdig_SYS_WINDOWS
#  ifndef xdig_OPT_WINDLL
#   if defined(_WINDLL)         /* This is used when building OpenSSL to
                                 * indicate that DLL linkage should be used */
#    define xdig_OPT_WINDLL
#   endif
#  endif
# endif

/* ------------------------------- OpenVMS -------------------------------- */
# if defined(__VMS) || defined(VMS)
#  if !defined(xdig_SYS_VMS)
#   undef xdig_SYS_UNIX
#   define xdig_SYS_VMS
#  endif
#  if defined(__DECC)
#   define xdig_SYS_VMS_DECC
#  elif defined(__DECCXX)
#   define xdig_SYS_VMS_DECC
#   define xdig_SYS_VMS_DECCXX
#  else
#   define xdig_SYS_VMS_NODECC
#  endif
# endif

/* -------------------------------- Unix ---------------------------------- */
# ifdef xdig_SYS_UNIX
#  if defined(linux) || defined(__linux__) && !defined(xdig_SYS_LINUX)
#   define xdig_SYS_LINUX
#  endif
#  if defined(_AIX) && !defined(xdig_SYS_AIX)
#   define xdig_SYS_AIX
#  endif
# endif

/* -------------------------------- VOS ----------------------------------- */
# if defined(__VOS__) && !defined(xdig_SYS_VOS)
#  define xdig_SYS_VOS
#  ifdef __HPPA__
#   define xdig_SYS_VOS_HPPA
#  endif
#  ifdef __IA32__
#   define xdig_SYS_VOS_IA32
#  endif
# endif

/* ---------------------------- HP NonStop -------------------------------- */
# ifdef __TANDEM
#  ifdef _STRING
#   include <strings.h>
#  endif
# define xdig_USE_BUILD_DATE
# if defined(xdig_THREADS) && defined(_SPT_MODEL_)
#  define  SPT_THREAD_SIGNAL 1
#  define  SPT_THREAD_AWARE 1
#  include <spthread.h>
# elif defined(xdig_THREADS) && defined(_PUT_MODEL_)
#  include <pthread.h>
# endif
# endif

/**
 * That's it for OS-specific stuff
 *****************************************************************************/

/* Standard integer types */
# define xdig_NO_INTTYPES_H
# define xdig_NO_STDINT_H
# if defined(xdig_SYS_UEFI)
typedef INT8 int8_t;
typedef UINT8 uint8_t;
typedef INT16 int16_t;
typedef UINT16 uint16_t;
typedef INT32 int32_t;
typedef UINT32 uint32_t;
typedef INT64 int64_t;
typedef UINT64 uint64_t;
typedef UINTN uintptr_t;
# elif (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L) || \
     defined(__osf__) || defined(__sgi) || defined(__hpux) || \
     defined(xdig_SYS_VMS) || defined (__OpenBSD__)
#  include <inttypes.h>
#  undef xdig_NO_INTTYPES_H
/* Because the specs say that inttypes.h includes stdint.h if present */
#  undef xdig_NO_STDINT_H
# elif defined(_MSC_VER) && _MSC_VER<1600
/*
 * minimally required typdefs for systems not supporting inttypes.h or
 * stdint.h: currently just older VC++
 */
typedef signed char int8_t;
typedef unsigned char uint8_t;
typedef short int16_t;
typedef unsigned short uint16_t;
typedef int int32_t;
typedef unsigned int uint32_t;
typedef __int64 int64_t;
typedef unsigned __int64 uint64_t;
# elif defined(xdig_SYS_TANDEM)
#  include <stdint.h>
#  include <sys/types.h>
# else
#  include <stdint.h>
#  undef xdig_NO_STDINT_H
# endif

#ifdef  __cplusplus
}
#endif
#endif
