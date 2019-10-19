set(LIBRARY_HEADERS
        include/derplib/container/cfq_parallel_consumer.h
        include/derplib/container/circular_queue.h)
set(LIBRARY_IMPLS
        include/derplib/container/cfq_parallel_consumer.ipp
        include/derplib/container/circular_queue.ipp)
set(LIBRARY_SOURCES)
set(TEST_SOURCES
        tests/cfq_parallel_consumer-test.cpp
        tests/circular_queue-test.cpp)

derplib_add_library(container
        HEADERS ${LIBRARY_HEADERS}
        IMPLS ${LIBRARY_IMPLS}
        SOURCES ${LIBRARY_SOURCES}
        LINK_DEPS derplib::internal derplib::stdext)

derplib_add_test(container
        SOURCES ${TEST_SOURCES})
