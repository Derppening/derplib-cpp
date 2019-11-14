find_package(Doxygen OPTIONAL_COMPONENTS dot)

if (${DOXYGEN_FOUND})
    set(DOXYGEN_WARNINGS YES)
    set(DOXYGEN_WARN_NO_PARAMDOC YES)
    set(DOXYGEN_WARN_IF_DOC_ERROR YES)
    set(DOXYGEN_WARN_IF_UNDOCUMENTED YES)
    set(DOXYGEN_GENERATE_TREEVIEW YES)
    set(DOXYGEN_GENERATE_HTML YES)
    set(DOXYGEN_GENERATE_MAN NO)
    set(DOXYGEN_ENABLE_PREPROCESSING NO)
    set(DOXYGEN_EXTRACT_ALL YES)

    # Check if Graphviz Dot is installed
    if (TARGET Doxygen::dot)
        set(DOXYGEN_HAVE_DOT YES)
        set(DOXYGEN_CALL_GRAPH YES)
        set(DOXYGEN_CALLER_GRAPH YES)
    else (TARGET Doxygen::dot)
        message(WARNING "Cannot find Graphviz dot: Documentation graphs will not be generated")
    endif (TARGET Doxygen::dot)

    set(DOXYGEN_FULL_PATH_NAMES YES)
    set(DOXYGEN_STRIP_FROM_PATH "${PROJECT_SOURCE_DIR}")
    set(DOXYGEN_EXCLUDE_PATTERNS
            "${PROJECT_BINARY_DIR}"
            "${PROJECT_SOURCE_DIR}/cmake-build-*"
            "${PROJECT_SOURCE_DIR}/*/tests")
    set(DOXYGEN_HTML_OUTPUT "${PROJECT_SOURCE_DIR}/docs")

    set(DOXYGEN_EXCLUDE_SYMBOLS
            derplib::internal)

    # Display the include path relative to the library
    set(DOXYGEN_STRIP_FROM_INC_PATH ${SUBDIR_TARGETS})
    list(TRANSFORM DOXYGEN_STRIP_FROM_INC_PATH REPLACE "(.+)" "${PROJECT_SOURCE_DIR}/\\0/include")

    doxygen_add_docs(derplib_doxygen ${PROJECT_SOURCE_DIR})
else (${DOXYGEN_FOUND})
    message(WARNING "Cannot find Doxygen: Documentation will not be generated")
endif (${DOXYGEN_FOUND})
