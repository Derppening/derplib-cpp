set(LIBRARY_HEADERS
        include/derplib/stdext/algorithm.h
        include/derplib/stdext/array.h
        include/derplib/stdext/cmath.h
        include/derplib/stdext/demangle.h
        include/derplib/stdext/iterator.h
        include/derplib/stdext/memory.h
        include/derplib/stdext/ptr.h
        include/derplib/stdext/ranges.h
        include/derplib/stdext/string.h
        include/derplib/stdext/type_traits.h
        include/derplib/stdext/version.h

        include/derplib/stdext/newlib/container.h
        include/derplib/stdext/newlib/memory.h)
set(LIBRARY_SOURCES
        src/string.cpp)
set(TEST_SOURCES
        tests/algorithm-test.cpp
        tests/array-test.cpp
        tests/cmath-test.cpp
        tests/demangle-test.cpp
        tests/iterator-test.cpp
        tests/memory-test.cpp
        tests/ptr-test.cpp
        tests/ranges-test.cpp
        tests/string-test.cpp
        tests/version-test.cpp

        tests/newlib/container-test.cpp
        tests/newlib/memory-test.cpp)

derplib_add_library(stdext
        HEADERS ${LIBRARY_HEADERS}
        SOURCES ${LIBRARY_SOURCES}
        LINK_DEPS derplib::internal)

derplib_add_test(stdext
        SOURCES ${TEST_SOURCES})