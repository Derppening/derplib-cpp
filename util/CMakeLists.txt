set(LIBRARY_HEADERS
        include/derplib/util/array.h
        include/derplib/util/demangle.h
        include/derplib/util/iterator.h
        include/derplib/util/ptr.h
        include/derplib/util/ranges.h
        include/derplib/util/stats.h)
set(LIBRARY_IMPLS
        include/derplib/util/array.ipp
        include/derplib/util/demangle.ipp
        include/derplib/util/ptr.ipp
        include/derplib/util/ranges.ipp
        include/derplib/util/stats.ipp)
set(LIBRARY_SOURCES)
set(TEST_SOURCES
        tests/array-test.cpp
        tests/demangle-test.cpp
        tests/iterator-test.cpp
        tests/ptr-test.cpp
        tests/ranges-test.cpp
        tests/stats-test.cpp)

derplib_add_library(util
        HEADERS ${LIBRARY_HEADERS}
        IMPLS ${LIBRARY_IMPLS}
        SOURCES ${LIBRARY_SOURCES}
        LINK_DEPS derplib::internal derplib::stdext)

derplib_add_test(util
        SOURCES ${TEST_SOURCES})