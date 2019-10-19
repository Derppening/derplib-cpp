set(LIBRARY_HEADERS
        include/derplib/base/log.h
        include/derplib/base/semver.h
        include/derplib/base/stopwatch.h
        include/derplib/base/timer.h)
set(LIBRARY_SOURCES
        src/semver.cpp
        src/stopwatch.cpp)
set(TEST_SOURCES
        tests/log-test.cpp
        tests/semver-test.cpp
        tests/stopwatch-test.cpp
        tests/timer-test.cpp)

derplib_add_library(base
        HEADERS ${LIBRARY_HEADERS}
        SOURCES ${LIBRARY_SOURCES}
        LINK_DEPS derplib::internal derplib::stdext derplib::util)

derplib_add_test(base
        SOURCES ${TEST_SOURCES})