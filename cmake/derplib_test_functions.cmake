# Taken from FMT as well
function(derplib_add_executable name)
    add_executable(${name} ${ARGN})
    if (MINGW)
        target_link_libraries(${name} -static-libgcc -static-libstdc++)
    endif ()
endfunction()

# Adds a test named "derplib_$name-test" with derplib::$name as a library dependency.
#
# This function will do nothing is ${DERPLIB_RUN_TESTS} is set to OFF.
#
# Referenced from FMT.
#
# Usage: derplib_add_test(name SOURCES srcs... LINK_DEPS lib_deps...)
function(derplib_add_test testname)
    if (${DERPLIB_RUN_TESTS})
        set(multiValueArgs HEADERS IMPLS SOURCES LINK_DEPS)
        cmake_parse_arguments(DERPLIB_TEST "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

        derplib_add_executable(derplib_${testname}-test ${DERPLIB_TEST_SOURCES})
        target_link_libraries(derplib_${testname}-test derplib::${testname} ${DERPLIB_TEST_LINK_DEPS} gtest_main)

        add_test(NAME derplib_${testname}-test COMMAND derplib_${testname}-test)
    endif (${DERPLIB_RUN_TESTS})
endfunction()