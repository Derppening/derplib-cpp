#pragma once

#include <cmath>
#include <type_traits>

namespace derplib {
inline namespace stdext {

#include <derplib/internal/common_macros_begin.h>

/**
 * \brief Converts a degree value to radians.
 *
 * \tparam T original data type
 * \tparam R returned data type. Must be a floating point type.
 * \param degrees original value in degrees
 * \return Radian value.
 *
 * \note `R` is mandated to be a floating point type, in order to avoid truncation when converted into an integer.
 */
template<typename T, typename R = T>
inline DERPLIB_CPP14_CONSTEXPR typename std::enable_if<std::is_floating_point<R>::value, R>::type to_radians(
    const T& degrees);

/**
 * \brief Converts a radian value to degrees.
 *
 * \tparam T data type
 * \param radians original value in radians
 * \return Degree value.
 */
template<typename T>
inline DERPLIB_CPP14_CONSTEXPR typename std::enable_if<std::is_floating_point<T>::value, T>::type to_degrees(
    const T& radians);

#if !defined(M_PI)
#define M_PI 3.141592653589793238462643383279502
#endif  // !defined(M_PI)

template<typename T, typename R>
inline DERPLIB_CPP14_CONSTEXPR typename std::enable_if<std::is_floating_point<R>::value, R>::type to_radians(
    const T& degrees) {
  constexpr R degree_to_radian = static_cast<R>(M_PI) / 180.0;

  return degrees * degree_to_radian;
}

template<typename T>
inline DERPLIB_CPP14_CONSTEXPR typename std::enable_if<std::is_floating_point<T>::value, T>::type to_degrees(
    const T& radians) {
  constexpr T radian_to_degree = 180.0 / static_cast<T>(M_PI);

  return radians * radian_to_degree;
}

#include <derplib/internal/common_macros_end.h>

}  // namespace stdext
}  // namespace derplib
