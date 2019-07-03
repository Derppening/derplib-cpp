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

}  // namespace newlib
}  // namespace derplib
