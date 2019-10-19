set(LIBRARY_HEADERS
        include/derplib/stdext/algorithm.h
        include/derplib/stdext/cmath.h
        include/derplib/stdext/memory.h
        include/derplib/stdext/string.h
        include/derplib/stdext/type_traits.h
        include/derplib/stdext/version.h)
set(LIBRARY_IMPLS
        include/derplib/stdext/algorithm.ipp
        include/derplib/stdext/cmath.ipp
        include/derplib/stdext/memory.ipp
        include/derplib/stdext/string.ipp)
set(LIBRARY_SOURCES
        src/string.cpp)
set(TEST_SOURCES
        tests/algorithm-test.cpp
        tests/cmath-test.cpp
        tests/memory-test.cpp
        tests/string-test.cpp
        tests/version-test.cpp)

derplib_add_library(stdext
        HEADERS ${LIBRARY_HEADERS}
        IMPLS ${LIBRARY_IMPLS}
        SOURCES ${LIBRARY_SOURCES}
        LINK_DEPS derplib::internal)

derplib_add_test(stdext
        SOURCES ${TEST_SOURCES})