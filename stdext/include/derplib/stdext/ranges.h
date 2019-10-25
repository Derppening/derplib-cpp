// Utilities for ranges.

#pragma once

#include <cassert>
#include <cmath>
#include <functional>
#include <numeric>
#include <sstream>
#include <type_traits>
#include <vector>

#include "type_traits.h"

namespace derplib {
inline namespace stdext {

#include <derplib/internal/common_macros_begin.h>

/**
 * \brief Generates a int range for [start,end).
 *
 * \param start Start number.
 * \param end End number (exclusive).
 * \param step Number to step per iteration.
 * \return A std::vector containing all numbers in the range.
 */
template<typename T, typename = std::enable_if<std::is_integral<T>::value>>
std::vector<T> int_range(const T& start, const T& end, const T& step);

/**
 * \brief Generates a sequence with sequentially increasing values starting with `value`.
 * \param value Initial value to store.
 * \return An std::array containing the sequence.
 */
template<typename T, std::size_t N, typename = std::enable_if<std::is_integral<T>::value>>
std::array<T, N> generate_sequence(const T& value);

/**
 * \brief Generates a sequence with sequentially increasing values starting with `value`.
 * \param size Size of sequence to generate.
 * \param value Initial value to store.
 * \return A std::vector containing the sequence.
 */
template<typename T, typename = std::enable_if<std::is_integral<T>::value>>
std::vector<T> generate_sequence(std::size_t size, const T& value);

template<typename T, typename>
std::vector<T> int_range(const T& start, const T& end, const T& step) {
  assert(end - start >= 0 && step >= 0 || end - start < 0 && step < 0);

  std::vector<T> v(static_cast<typename std::vector<T>::size_type>(std::ceil((end - start) / float(step))));
  std::generate(v.begin(), v.end(), [=, &start]() {
    T n = start;
    start += step;
    return n;
  });
  return v;
}

template<typename T, std::size_t N, typename>
std::array<T, N> generate_sequence(const T& value) {
  std::array<T, N> array = {};
  std::iota(array.begin(), array.end(), value);
  return array;
}

template<typename T, typename>
std::vector<T> generate_sequence(const std::size_t size, const T& value) {
  std::vector<T> v(size);
  std::iota(v.begin(), v.end(), value);
  return v;
}

#include <derplib/internal/common_macros_end.h>

}  // namespace stdext
}  // namespace derplib
