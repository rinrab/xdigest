# extractor.cmake -- Extracts function names from header files and writes
# them to a .def file.
#
# cmake -DEXPORT_HEADER_FILE_PATHS=<...path_to_header_files>
#       -DEXPORT_DEF_FILE_PATH=<path_to_def_file>
#       -P extractor.cmake

separate_arguments(EXPORT_HEADER_FILE_PATHS)

set(func_regex "(^|\n)((([A-Za-z0-9_]+|[*]) )+[*]?)?((xdig)_[A-Za-z0-9_]+)[ \t\r\n]*\\(")

set(defs)
foreach(file ${EXPORT_HEADER_FILE_PATHS})
    file(READ ${file} contents)
    string(REGEX MATCHALL "${func_regex}" funcs ${contents})

    foreach(func_string ${funcs})
        string(REGEX MATCH "[A-Za-z0-9_]+[ \t\r\n]*\\($" func_name ${func_string})
        string(REGEX REPLACE "[ \t\r\n]*\\($" "" func_name ${func_name})
        list(APPEND defs "${func_name}")
    endforeach()
endforeach()

list(SORT defs)
list(REMOVE_DUPLICATES defs)

set(def_file_content "EXPORTS\n")
foreach(def ${defs})
    list(FIND EXPORT_BLACKLIST "${def}" skip)
    if(skip LESS 0)
        string(APPEND def_file_content "${def}\n")
    endif()
endforeach()

if(EXISTS "${EXPORT_DEF_FILE_PATH}")
    file(READ "${EXPORT_DEF_FILE_PATH}" old_file_content)
else()
    set(old_file_content "NON_EXISTING")
endif()
if(NOT ${old_file_content} STREQUAL ${def_file_content})
    file(WRITE "${EXPORT_DEF_FILE_PATH}" ${def_file_content})
endif()
