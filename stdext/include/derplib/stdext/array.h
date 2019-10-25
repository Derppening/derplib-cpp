// Utilities for arrays.

#pragma once

#include <array>
#include <iterator>

namespace derplib {
inline namespace stdext {

#include <derplib/internal/common_macros_begin.h>

/**
 * Converts a C-style array to C++11-style \c std::array.
 *
 * \tparam T Array data type
 * \tparam N Size of the array
 * \param arr Original C-style array
 * \return \p arr in \c std::array
 */
template<typename T, std::size_t N>
DERPLIB_CPP14_CONSTEXPR std::array<T, N> to_array(const T (&arr)[N]) noexcept;

/**
 * Converts a C-style array to C++11-style \c std::array.
 *
 * \tparam T Array data type
 * \tparam N Size of the array
 * \param arr Original C-style array
 * \return \p arr in \c std::array
 */
template<typename T, std::size_t N>
DERPLIB_CPP14_CONSTEXPR std::array<T, N> to_array(T(&&arr)[N]) noexcept;

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
template<typename T, std::size_t N>
constexpr std::size_t size(DERPLIB_MAYBE_UNUSED T (&arr)[N]) {
  return N;
}

/**
 * \brief Template function which returns the size of an STL container.
 *
 * This function is intended to be a compatibility function between pre-C++17 and C++17.
 *
 * \tparam C Container type which must contain a `size` member function.
 * \param c a container
 * \return The size of `c`.
 */
template<typename C>
constexpr std::size_t size(const C& c) {
  return c.size();
}

template<typename T, std::size_t N>
DERPLIB_CPP14_CONSTEXPR std::array<T, N> to_array(const T (&arr)[N]) noexcept {
  std::array<T, N> a;
  std::copy(std::begin(arr), std::end(arr), a.begin());
  return a;
}

template<typename T, std::size_t N>
DERPLIB_CPP14_CONSTEXPR std::array<T, N> to_array(T(&&arr)[N]) noexcept {
  std::array<T, N> a;
  for (std::size_t i = 0; i < N; ++i) {
    a[i] = std::move(arr[i]);
  }
  return a;
}

#include <derplib/internal/common_macros_end.h>

}  // namespace stdext
}  // namespace derplib
