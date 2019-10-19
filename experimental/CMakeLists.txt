set(LIBRARY_HEADERS
        include/derplib/experimental/config_file.h
        include/derplib/experimental/cli.h
        include/derplib/experimental/ref_count_pointer.h
        include/derplib/experimental/mem_alloc/bad_alloc.h
        include/derplib/experimental/mem_alloc/fixed_pool_mem_alloc.h
        include/derplib/experimental/mem_alloc/heap_walk_iterator.h)
set(LIBRARY_IMPLS
        include/derplib/experimental/mem_alloc/fixed_pool_mem_alloc.ipp)
set(LIBRARY_SOURCES
        src/cli.cpp
        src/config_file.cpp
        src/mem_alloc/fixed_pool_mem_alloc.cpp
        src/mem_alloc/mem_alloc_exceptions.cpp)
set(TEST_SOURCES
        tests/cli-test.cpp
        tests/config_file-test.cpp
        tests/ref_count_pointer-test.cpp)

derplib_add_library(experimental
        HEADERS ${LIBRARY_HEADERS}
        IMPLS ${LIBRARY_IMPLS}
        SOURCES ${LIBRARY_SOURCES}
        LINK_DEPS derplib::internal derplib::stdext derplib::util)

derplib_add_test(experimental
        SOURCES ${TEST_SOURCES})