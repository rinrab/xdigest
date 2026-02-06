#
# FindXDIGEST.cmake: a cmake module to check for xdigest dependency
#
# Written by Timofei Zhakov, 2026. No rights have been reseved.
#
# This files is unlicensed. You're absolutely free to steal it if you use
# xdigest as a dependency. No credits for this specific file are needed. And if
# you wish you could even strip away this header.
#
# ====================================================================
# This is free and unencumbered software released into the public domain.
# 
# Anyone is free to copy, modify, publish, use, compile, sell, or
# distribute this software, either in source code form or as a compiled
# binary, for any purpose, commercial or non-commercial, and by any
# means.
# 
# In jurisdictions that recognize copyright laws, the author or authors
# of this software dedicate any and all copyright interest in the
# software to the public domain. We make this dedication for the benefit
# of the public at large and to the detriment of our heirs and
# successors. We intend this dedication to be an overt act of
# relinquishment in perpetuity of all present and future rights to this
# software under copyright law.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
# IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
# OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
# ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
# OTHER DEALINGS IN THE SOFTWARE.
# 
# For more information, please refer to <https://unlicense.org/>
# ====================================================================
#

find_path(XDIGEST_INCLUDE_DIR
  NAMES "xdigest.h"
  PATH_SUFFIXES
    "include/xdigest"
)

find_library(XDIGEST_LIBRARY
  NAMES "xdigest"
  PATH_SUFFIXES "lib"
)

if(XDIGEST_INCLUDE_DIR AND EXISTS "${XDIGEST_INCLUDE_DIR}/xdigest.h")
    file(
      STRINGS "${XDIGEST_INCLUDE_DIR}/xdigest.h" VERSION_STRINGS
      REGEX "#define (XDIG_VERSION_MAJOR|XDIG_VERSION_MINOR|XDIG_VERSION_PATCH)"
    )

    string(REGEX REPLACE ".*XDIG_VERSION_MAJOR +([0-9]+).*" "\\1" XDIG_VERSION_MAJOR ${VERSION_STRINGS})
    string(REGEX REPLACE ".*XDIG_VERSION_MINOR +([0-9]+).*" "\\1" XDIG_VERSION_MINOR ${VERSION_STRINGS})
    string(REGEX REPLACE ".*XDIG_VERSION_PATCH +([0-9]+).*" "\\1" XDIG_VERSION_PATCH ${VERSION_STRINGS})

    set(XDIGEST_VERSION "${XDIG_VERSION_MAJOR}.${XDIG_VERSION_MINOR}.${XDIG_VERSION_PATCH}")
endif()

mark_as_advanced(
  XDIGEST_INCLUDE_DIR
  XDIGEST_LIBRARY
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
  XDIGEST
  REQUIRED_VARS
    XDIGEST_LIBRARY
    XDIGEST_INCLUDE_DIR
  VERSION_VAR
    XDIGEST_VERSION
)

if(XDIGEST_FOUND)
  add_library(XDIGEST::XDIGEST IMPORTED STATIC)

  set_target_properties(XDIGEST::XDIGEST PROPERTIES
    IMPORTED_LOCATION ${XDIGEST_LIBRARY}
    INTERFACE_INCLUDE_DIRECTORIES ${XDIGEST_INCLUDE_DIR}
  )
endif()
