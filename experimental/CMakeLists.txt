set(LIBRARY_HEADERS
        include/derplib/experimental/config_file.h
        include/derplib/experimental/cli.h
        include/derplib/experimental/ref_count_pointer.h
        include/derplib/experimental/heap_pool_allocator/base_heap_pool.h
        include/derplib/experimental/heap_pool_allocator/simple_pool_allocator.h)
set(LIBRARY_SOURCES
        src/cli.cpp
        src/config_file.cpp
        src/heap_pool_allocator/simple_pool_allocator.cpp)
set(TEST_SOURCES
        tests/heap_pool_allocator/simple_pool_allocator.cpp)

derplib_add_library(experimental
        HEADERS ${LIBRARY_HEADERS}
        SOURCES ${LIBRARY_SOURCES}
        LINK_DEPS derplib::internal derplib::stdext)

derplib_add_test(experimental
        SOURCES ${TEST_SOURCES})