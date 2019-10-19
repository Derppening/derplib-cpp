#include <cassert>
#include <algorithm>
#include <functional>

namespace derplib {
namespace stdext {

#include "derplib/internal/common_macros_begin.h"

template<class T, class Compare>
inline DERPLIB_CPP14_CONSTEXPR const T& clamp(const T& v, const T& lo, const T& hi, Compare comp) {
  return assert(!comp(hi, lo)), comp(v, lo) ? lo : comp(hi, v) ? hi : v;
}

template<class T>
inline DERPLIB_CPP14_CONSTEXPR const T& clamp(const T& v, const T& lo, const T& hi) {
  return clamp(v, lo, hi, std::less<T>());
}

template<class T, class Compare>
DERPLIB_CPP14_CONSTEXPR bool in_range(const T& v, const T& lo, const T& hi, Compare comp) {
  return assert(!comp(hi, lo)), comp(v, lo) ? false : comp(hi, v) ? false : true;
}

template<class T>
DERPLIB_CPP14_CONSTEXPR bool in_range(const T& v, const T& lo, const T& hi) {
  return in_range(v, lo, hi, std::less<T>());
}

#include "derplib/internal/common_macros_end.h"

}  // namespace stdext
}  // namespace derplib
