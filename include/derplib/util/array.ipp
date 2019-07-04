#include <array>
#include <iterator>

namespace derplib {
namespace util {

#include "derplib/internal/common_macros_begin.h"

template<typename T, std::size_t N>
DERPLIB_CPP14_CONSTEXPR std::array<T, N> to_array(T (& arr)[N]) {
  std::array<T, N> a;
  std::copy(arr, arr + N, a.begin());
  return a;
}

template<typename T, std::size_t N>
constexpr std::size_t size(DERPLIB_MAYBE_UNUSED T (& arr)[N]) {
  // TODO(Derppening): Move to stdext, add overloads
  return N;
}

#include "derplib/internal/common_macros_end.h"

}  // namespace util
}  // namespace derplib
