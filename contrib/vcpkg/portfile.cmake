vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO rinrab/xdigest
    REF "${VERSION}"
    SHA512 0
    HEAD_REF trunk
)

vcpkg_check_features(
    OUT_FEATURE_OPTIONS FEATURE_OPTIONS
    FEATURES
        asm USE_ASM
)

set(OPTION "")

if (NOT USE_ASM)
    list(APPEND OPTIONS NO_ASM=1)
endif()

if(VCPKG_TARGET_IS_LINUX)
    if(VCPKG_TARGET_ARCHITECTURE MATCHES "arm64")
        set(ENV{ARCH} aarch64)
    elseif(VCPKG_TARGET_ARCHITECTURE MATCHES "x64")
        set(ENV{ARCH} x86_64)
    elseif(VCPKG_TARGET_ARCHITECTURE MATCHES "x86")
        set(ENV{ARCH} x86)
    else()
        message(FATAL_ERROR "Unsupported architecture: ${VCPKG_TARGET_ARCHITECTURE}")
    endif()
endif()

if (VCPKG_TARGET_IS_WINDOWS)
    # todo
else()
    file(COPY "${CMAKE_CURRENT_LIST_DIR}/configure" DESTINATION "${SOURCE_PATH}")

    vcpkg_configure_make(
        SOURCE_PATH "${SOURCE_PATH}"
        COPY_SOURCE
        OPTIONS_RELEASE "--vcpkg-prefix=${CURRENT_PACKAGES_DIR}"
        OPTIONS_DEBUG "--vcpkg-prefix=${CURRENT_PACKAGES_DIR}/debug"
    )

    vcpkg_build_make(
        BUILD_TARGET install
        MAKEFILE Makefile.conf
    )
endif()

file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/include")

vcpkg_install_copyright(FILE_LIST "${SOURCE_PATH}/LICENSE")

