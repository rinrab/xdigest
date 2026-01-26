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

if (VCPKG_TARGET_IS_WINDOWS)
    # todo
else()
    vcpkg_configure_make(
        SOURCE_PATH "${SOURCE_PATH}"
        COPY_SOURCE
    )

    vcpkg_install_make(
        MAKEFILE Makefile.conf
    )
endif()

file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/include")

vcpkg_install_copyright(FILE_LIST "${SOURCE_PATH}/LICENSE")

