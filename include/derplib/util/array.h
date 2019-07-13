// Utilities for arrays.

#pragma once

#include <array>
#include <iterator>

namespace derplib {
namespace util {

#include "derplib/internal/common_macros_begin.h"

/**
 * Converts a C-style array to C++11-style \c std::array.
 *
 * \tparam T Array data type
 * \tparam N Size of the array
 * \param arr Original C-style array
 * \return \p arr in \c std::array
 */
template<typename T, std::size_t N>
DERPLIB_CPP14_CONSTEXPR std::array<T, N> to_array(T (& arr)[N]);

/**
 * Template function which returns the size of a C-style array
 *
 * This function is to facilitate the lack of a \c .size() function for C-style arrays.
 *
 * \tparam T Array data type.
 * \tparam N Size of the array.
 * \param arr A C-style array
 * \return Size of the C-style array
 */
// TODO(Derppening): Move to stdext as drop-in backport for std::size
template<typename T, std::size_t N>
constexpr std::size_t size(DERPLIB_MAYBE_UNUSED T (& arr)[N]);

#include "derplib/internal/common_macros_end.h"

}  // namespace util
}  // namespace derplib

#include "derplib/util/array.ipp"
