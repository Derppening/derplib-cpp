set(LIBRARY_HEADERS
        include/derplib/newlib/container.h
        include/derplib/newlib/memory.h)
set(LIBRARY_IMPLS
        include/derplib/newlib/container.ipp
        include/derplib/newlib/memory.ipp)
set(LIBRARY_SOURCES)
set(TEST_SOURCES
        tests/container-test.cpp
        tests/memory-test.cpp)

derplib_add_library(newlib
        HEADERS ${LIBRARY_HEADERS}
        IMPLS ${LIBRARY_IMPLS}
        SOURCES ${LIBRARY_SOURCES})

derplib_add_test(newlib
        SOURCES ${TEST_SOURCES}
        LINK_DEPS derplib::stdext)
