#include <cmath>
#include <algorithm>
#include <numeric>
#include <vector>

namespace derplib {
namespace util {

#include <derplib/internal/common_macros_begin.h>

template<typename T, typename>
std::vector<T> int_range(const T& start, const T& end, const T& step) {
  assert(end - start >= 0 && step >= 0 || end - start < 0 && step < 0);

  std::vector<T> v(typename std::vector<T>::size_type(std::ceil((end - start) / float(step))));
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

}  // namespace util
}  // namespace derplib
