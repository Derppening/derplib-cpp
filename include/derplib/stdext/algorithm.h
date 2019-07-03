#pragma once

namespace derplib {
namespace stdext {

#include "derplib/internal/common_macros_begin.h"

/**
 * \brief Backport of `std::clamp` from C++17.
 *
 * \tparam T comparison type
 * \tparam Compare comparison function type
 * \param v the value to clamp
 * \param lo lower bound
 * \param hi upper bound
 * \param comp comparison function object
 * \return Reference to lo if v is less than lo, reference to hi if hi is less than hi, otherwise reference to v.
 */
template<class T, class Compare>
inline DERPLIB_CPP14_CONSTEXPR const T& clamp(const T& v, const T& lo, const T& hi, Compare comp);

/**
 * \brief Backport of `std::clamp` from C++17.
 *
 * \tparam T comparison type
 * \param v the value to clamp
 * \param lo lower bound
 * \param hi upper bound
 * \return Reference to lo if v is less than lo, reference to hi if hi is less than hi, otherwise reference to v.
 */
template<class T>
inline DERPLIB_CPP14_CONSTEXPR const T& clamp(const T& v, const T& lo, const T& hi);

/**
 * @brief Checks whether a value is in range.
 *
 * @tparam T value type
 * @tparam Compare Comparator. Should return true if arg0 < arg1.
 * @param v Value to check.
 * @param lo Lower bound of clamp.
 * @param hi Upper bound of clamp.
 * @param comp Comparator function.
 * @return Whether v is clamped between lo and hi.
 */
template<class T, class Compare>
DERPLIB_CPP14_CONSTEXPR bool in_range(const T& v, const T& lo, const T& hi, Compare comp);

/**
 * @brief Returns whether a value has been clamped.
 *
 * @tparam T value type
 * @param v Value to check.
 * @param lo Lower bound of clamp.
 * @param hi Upper bound of clamp.
 * @return Whether v is clamped between lo and hi.
 */
template<class T>
DERPLIB_CPP14_CONSTEXPR bool in_range(const T& v, const T& lo, const T& hi);

#include "derplib/internal/common_macros_end.h"

}  // namespace stdext
}  // namespace derplib

#include "derplib/stdext/algorithm.ipp"
