set(LIBRARY_HEADERS
        include/derplib/internal/common_macros_begin.h
        include/derplib/internal/common_macros_end.h)
set(LIBRARY_IMPLS)
set(LIBRARY_SOURCES)

derplib_add_library(internal
        HEADERS ${LIBRARY_HEADERS}
        IMPLS ${LIBRARY_IMPLS}
        SOURCES ${LIBRARY_SOURCES}
        LINK_DEPS derplib::stdext derplib::util)