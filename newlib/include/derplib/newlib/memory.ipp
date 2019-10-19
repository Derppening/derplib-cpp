#include <memory>

namespace derplib {
namespace newlib {

template<typename T, typename Deleter>
T* element_at(const std::unique_ptr<T[], Deleter>& uptr, std::size_t index) noexcept {
  T* const p = uptr.get();
  if (p == nullptr) {
    return nullptr;
  }

  return &p[index];
}

template<typename T, typename Deleter, typename R, typename... Args>
std::unique_ptr<R> invoke(const std::unique_ptr<T, Deleter>& uptr, R (T::*func)(Args...), Args&&... args) noexcept {
  if (uptr && func) {
    return derplib::stdext::make_unique<R>(uptr->func(args...));
  } else {
    return nullptr;
  }
}

}  // namespace newlib
}  // namespace derplib
