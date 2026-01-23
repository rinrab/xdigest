vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO rinrab/xhash
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
else()
    vcpkg_install_make(
        SOURCE_PATH "${SOURCE_PATH}"
        OPTIONS ${OPTIONS}
    )
endif()

vcpkg_install_copyright(FILE_LIST "${SOURCE_PATH}/LICENSE")

