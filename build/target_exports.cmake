function(target_exports target_name)
    if (WIN32)
        set(def_file_path "${CMAKE_CURRENT_BINARY_DIR}/${target_name}.def")
        set(headers ${ARGN})

        add_custom_command(
            OUTPUT "${def_file_path}"
            DEPENDS ${headers}
            COMMAND ${CMAKE_COMMAND}
                    "-DEXPORT_HEADER_FILE_PATHS=${headers}"
                    "-DEXPORT_DEF_FILE_PATH=${def_file_path}"
                    -P "${CMAKE_CURRENT_SOURCE_DIR}/build/extractor.cmake"
            WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
        )

        target_sources("${target_name}" PRIVATE "${def_file_path}")
    endif()
endfunction()
