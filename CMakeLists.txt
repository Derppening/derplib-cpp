cmake_minimum_required(VERSION 3.3)
project(derplib
        VERSION 0.1.0)

cmake_policy(SET CMP0012 NEW)
cmake_policy(SET CMP0077 NEW)

# Include warning flags
include(cmake/compile_flags.cmake)

function(derplib_add_headers VAR)
    set(headers ${${VAR}})
    foreach (header ${ARGN})
        set(headers ${headers} include/derplib/${header})
    endforeach ()
    set(${VAR} ${headers} PARENT_SCOPE)
endfunction()

function(derplib_add_sources VAR)
    set(sources ${${VAR}})
    foreach (source ${ARGN})
        set(sources ${sources} src/${source})
    endforeach ()
    set(${VAR} ${sources} PARENT_SCOPE)
endfunction()

# derplib_add_library(NAME LINK_DEPENDENCIES...)
function(derplib_add_library NAME)
    # Add Targets
    add_library(derplib_${NAME} ${DERPLIB_${NAME}_HEADERS} ${DERPLIB_${NAME}_IMPLS} ${DERPLIB_${NAME}_SOURCES})
    add_library(derplib::${NAME} ALIAS derplib_${NAME})
    target_compile_features(derplib_${NAME} PUBLIC cxx_std_${CMAKE_CXX_STANDARD})

    # Set the warning flags
    if (${DERPLIB_WARN})
        target_compile_options(derplib_${NAME} PRIVATE ${CXX_WARN_FLAGS})
        target_include_directories(derplib_${NAME}
                PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)
    else (${DERPLIB_WARN})
        target_include_directories(derplib_${NAME} SYSTEM
                PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)
    endif (${DERPLIB_WARN})

    # Set target properties
    set_target_properties(derplib_${NAME} PROPERTIES LINKER_LANGUAGE CXX)
    target_link_libraries(derplib_${NAME} ${ARGN})
    if (MINGW)
        target_link_libraries(derplib_${NAME} -static-libgcc -static-libstdc++)
    endif ()
endfunction()

# Disable tests and warnings if we are only building this project
if (NOT CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    option(DERPLIB_BUILD_DOCS "Builds documentation using Doxygen." OFF)
    option(DERPLIB_RUN_TESTS "Runs Derplib tests" OFF)
    option(DERPLIB_WARN "Displays all warnings when compiling Derplib" OFF)
else ()
    option(DERPLIB_BUILD_DOCS "Builds documentation using Doxygen." ON)
    option(DERPLIB_RUN_TESTS "Runs Derplib tests" ON)
    option(DERPLIB_WARN "Displays all warnings when compiling Derplib" ON)
endif ()

# Set the C++ standard version from the parent project, from DERPLIB_CXX_STD_OVERRIDE if defined, else default to C++11
unset(DERPLIB_TEMP_CXX_TARGET)
if (DERPLIB_CXX_STD_OVERRIDE)
    set(DERPLIB_TEMP_CXX_TARGET)
    set(CMAKE_CXX_STANDARD ${DERPLIB_CXX_STD_OVERRIDE})
endif (DERPLIB_CXX_STD_OVERRIDE)
if (NOT CMAKE_CXX_STANDARD)
    set(DERPLIB_TEMP_CXX_TARGET)
    set(CMAKE_CXX_STANDARD 11)
endif (NOT CMAKE_CXX_STANDARD)

message(STATUS "Derplib CXX_STANDARD: ${CMAKE_CXX_STANDARD}")
message(STATUS "Derplib Build Docs: ${DERPLIB_BUILD_DOCS}")
message(STATUS "Derplib Run GTest: ${DERPLIB_RUN_TESTS}")
message(STATUS "Derplib Warnings: ${DERPLIB_WARN}")

# derplib::base
derplib_add_headers(DERPLIB_base_HEADERS
        base/log.h
        base/semver.h
        base/stopwatch.h
        base/timer.h)
derplib_add_headers(DERPLIB_base_IMPLS
        base/log.ipp
        base/timer.ipp)
derplib_add_sources(DERPLIB_base_SOURCES
        base/semver.cpp
        base/stopwatch.cpp)
derplib_add_library(base derplib_stdext derplib_util)

# derplib::container
derplib_add_headers(DERPLIB_container_HEADERS
        container/cfq_parallel_consumer.h
        container/circular_queue.h)
derplib_add_library(container)

# derplib::experimental
derplib_add_headers(DERPLIB_experimental_HEADERS
        experimental/config_file.h
        experimental/cli.h
        experimental/ref_count_pointer.h)
derplib_add_sources(DERPLIB_experimental_SOURCES
        experimental/config_file.cpp
        experimental/cli.cpp)
derplib_add_library(experimental)

# derplib::newlib
derplib_add_headers(DERPLIB_newlib_HEADERS
        newlib/container.h
        newlib/memory.h)
derplib_add_headers(DERPLIB_newlib_IMPLS
        newlib/container.ipp
        newlib/memory.ipp)
derplib_add_library(newlib)

# derplib::stdext
derplib_add_headers(DERPLIB_stdext_HEADERS
        stdext/algorithm.h
        stdext/cmath.h
        stdext/memory.h
        stdext/version.h)
derplib_add_headers(DERPLIB_stdext_IMPLS
        stdext/algorithm.ipp
        stdext/cmath.ipp
        stdext/memory.ipp)
derplib_add_library(stdext)

# derplib::util
derplib_add_headers(DERPLIB_util_HEADERS
        util/array.h
        util/demangle.h
        util/iterator.h
        util/ptr.h
        util/ranges.h
        util/stats.h
        util/string.h)
derplib_add_headers(DERPLIB_util_IMPLS
        util/array.ipp
        util/demangle.ipp
        util/ptr.ipp
        util/ranges.ipp
        util/stats.ipp
        util/string.ipp)
derplib_add_sources(DERPLIB_util_SOURCES
        util/string.cpp)
derplib_add_library(util)

# Doxygen Target
if (${DERPLIB_BUILD_DOCS})
    find_package(Doxygen OPTIONAL_COMPONENTS dot)

    if (${DOXYGEN_FOUND})
        set(DOXYGEN_WARNINGS YES)
        set(DOXYGEN_WARN_NO_PARAMDOC YES)
        set(DOXYGEN_WARN_IF_DOC_ERROR YES)
        set(DOXYGEN_WARN_IF_UNDOCUMENTED YES)
        set(DOXYGEN_GENERATE_TREEVIEW YES)
        set(DOXYGEN_GENERATE_HTML YES)
        set(DOXYGEN_GENERATE_MAN NO)
        set(DOXYGEN_ENABLE_PREPROCESSING NO)
        set(DOXYGEN_EXTRACT_ALL YES)

        # Check if Graphviz Dot is installed
        if (TARGET Doxygen::dot)
            set(DOXYGEN_HAVE_DOT YES)
            set(DOXYGEN_CALL_GRAPH YES)
            set(DOXYGEN_CALLER_GRAPH YES)
        else (TARGET Doxygen::dot)
            message(WARNING "Cannot find Graphviz dot: Documentation graphs will not be generated")
        endif (TARGET Doxygen::dot)

        set(DOXYGEN_FULL_PATH_NAMES YES)
        set(DOXYGEN_STRIP_FROM_PATH "${PROJECT_SOURCE_DIR}")
        set(DOXYGEN_EXCLUDE_PATTERNS
                "${PROJECT_BINARY_DIR}"
                "${PROJECT_SOURCE_DIR}/cmake-build-*"
                "${PROJECT_SOURCE_DIR}/tests")
        set(DOXYGEN_HTML_OUTPUT "${PROJECT_SOURCE_DIR}/docs")

        set(DOXYGEN_EXCLUDE_SYMBOLS
                derplib::internal)

        doxygen_add_docs(derplib_doxygen ${PROJECT_SOURCE_DIR})
    else (${DOXYGEN_FOUND})
        message(WARNING "Cannot find Doxygen: Documentation will not be generated")
    endif (${DOXYGEN_FOUND})
endif (${DERPLIB_BUILD_DOCS})

# GTest Target
if (${DERPLIB_RUN_TESTS})
    enable_testing()
    add_subdirectory(tests)
endif (${DERPLIB_RUN_TESTS})

# Unset the temporary C++ standard override
if (DERPLIB_TEMP_CXX_TARGET)
    unset(CMAKE_CXX_STANDARD)
    unset(DERPLIB_TEMP_CXX_TARGET)
endif (DERPLIB_TEMP_CXX_TARGET)
