project(derplib_tests CXX)

# Taken from FMT as well
function(derplib_add_executable name)
    add_executable(${name} ${ARGN})
    if (MINGW)
        target_link_libraries(${name} -static-libgcc -static-libstdc++)
    endif ()
endfunction()

# Adds a test named "derplib_$name-test" with derplib::$name as a library dependency.
# Referenced from FMT.
# Usage: derplib_add_test(name srcs...)
function(derplib_add_test testname)
    derplib_add_executable(derplib_${testname}-test ${ARGN})
    target_link_libraries(derplib_${testname}-test derplib::${testname} gtest_main)

    add_test(NAME derplib_${testname}-test COMMAND derplib_${testname}-test)
endfunction()

# Download and unpack googletest at configure time
configure_file(../cmake/config/GoogleTest-CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download)
if(result)
    message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download)
if(result)
    message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
        ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
        EXCLUDE_FROM_ALL)

# The gtest/gtest_main targets carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later. Otherwise we have to add them here ourselves.
if (CMAKE_VERSION VERSION_LESS 2.8.11)
    include_directories("${gtest_SOURCE_DIR}/include")
endif()

# derplib::base
derplib_add_test(base
        base/log-test.cpp
        base/semver-test.cpp
        base/stopwatch-test.cpp
        base/timer-test.cpp)

# derplib::container
derplib_add_test(container
        container/cfq_parallel_consumer-test.cpp
        container/circular_queue-test.cpp)

# derplib::experimental
derplib_add_test(experimental
        experimental/cli-test.cpp
        experimental/config_file-test.cpp
        experimental/ref_count_pointer-test.cpp)

# derplib::newlib
derplib_add_test(newlib
        newlib/container-test.cpp
        newlib/memory-test.cpp)

# derplib::stdext
derplib_add_test(stdext
        stdext/algorithm-test.cpp
        stdext/cmath-test.cpp
        stdext/memory-test.cpp
        stdext/string-test.cpp
        stdext/version-test.cpp)

# derplib::util
derplib_add_test(util
        util/array-test.cpp
        util/demangle-test.cpp
        util/iterator-test.cpp
        util/ptr-test.cpp
        util/ranges-test.cpp
        util/stats-test.cpp)
