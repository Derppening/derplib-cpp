#include <type_traits>

#if !defined(M_PI)
#define M_PI 3.141592653589793238462643383279502
#endif  // !defined(M_PI)

namespace derplib {
namespace stdext {

#include <derplib/internal/common_macros_begin.h>

template<typename T, typename R>
inline DERPLIB_CPP14_CONSTEXPR typename std::enable_if<std::is_floating_point<R>::value, R>::type to_radians(
    const T& degrees) {
  constexpr R degree_to_radian = R(M_PI) / 180.0;

  return degrees * degree_to_radian;
}

template<typename T>
inline DERPLIB_CPP14_CONSTEXPR typename std::enable_if<std::is_floating_point<T>::value, T>::type to_degrees(
    const T& radians) {
  constexpr T radian_to_degree = 180.0 / T(M_PI);

  return radians * radian_to_degree;
}

#include <derplib/internal/common_macros_end.h>

}  // namespace stdext
}  // namespace derplib
