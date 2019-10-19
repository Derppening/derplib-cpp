# Adds a library named "derplib_${NAME}".
#
# Usage: derplib_add_library(name HEADERS headers... IMPLS ipps... SOURCES srcs... LINK_DEPS lib_deps...)
function(derplib_add_library NAME)
    set(multiValueArgs HEADERS IMPLS SOURCES LINK_DEPS)
    cmake_parse_arguments(DERPLIB_LIBRARY "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    # Add Targets
    add_library(derplib_${NAME}
            ${DERPLIB_LIBRARY_HEADERS}
            ${DERPLIB_LIBRARY_IMPLS}
            ${DERPLIB_LIBRARY_SOURCES})
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
    target_link_libraries(derplib_${NAME} ${DERPLIB_LIBRARY_LINK_DEPS})
    if (MINGW)
        target_link_libraries(derplib_${NAME} -static-libgcc -static-libstdc++)
    endif ()
endfunction()
