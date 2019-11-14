cmake_minimum_required(VERSION 3.3)
project(derplib
        VERSION 0.1.0)

cmake_policy(SET CMP0012 NEW)
cmake_policy(SET CMP0077 NEW)

# Include helper functions for adding libraries and tests.
include(cmake/derplib_library_functions.cmake)
include(cmake/derplib_test_functions.cmake)

# Include warning flags
include(cmake/compile_flags.cmake)

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

# Add GTest configuration if testing is enabled. We will add targets later.
if (${DERPLIB_RUN_TESTS})
    enable_testing()
    include(cmake/gtest.cmake)
endif (${DERPLIB_RUN_TESTS})

# Add targets here.
set(SUBDIR_TARGETS
        base
        container
        experimental
        internal
        stdext)

foreach(TARGET ${SUBDIR_TARGETS})
    add_subdirectory(${TARGET})
endforeach()

# Doxygen Target
if (${DERPLIB_BUILD_DOCS})
    include(cmake/derplib_doxygen_configuration.cmake)
endif (${DERPLIB_BUILD_DOCS})

# Unset the temporary C++ standard override
if (DERPLIB_TEMP_CXX_TARGET)
    unset(CMAKE_CXX_STANDARD)
    unset(DERPLIB_TEMP_CXX_TARGET)
endif (DERPLIB_TEMP_CXX_TARGET)
