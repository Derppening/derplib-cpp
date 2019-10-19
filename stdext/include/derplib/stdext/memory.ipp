#include <memory>
#include <type_traits>

#include "version.h"

namespace derplib {
namespace stdext {

template<typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args) {
  return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

template<typename T>
std::unique_ptr<T> make_unique(std::size_t size) {
  return std::unique_ptr<T>(new typename std::remove_extent<T>::type[size]());
}

}  // namespace stdext
}  // namespace derplib
