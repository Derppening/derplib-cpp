set(LIBRARY_HEADERS
        include/derplib/internal/common_macros_begin.h
        include/derplib/internal/common_macros_end.h)
set(LIBRARY_SOURCES)

derplib_add_library(internal
        HEADERS ${LIBRARY_HEADERS}
        SOURCES ${LIBRARY_SOURCES}
        LINK_DEPS derplib::stdext derplib::util)