#include "derplib/internal/common_macros_begin.h"

namespace derplib {
namespace experimental {

template<typename T, typename... Args>
T* mem_alloc::allocate(Args&& ... args) {
  unsigned char* const ptr = find_first_fit(sizeof(T));
  if (ptr == nullptr) {
    heap_dump();
    throw bad_alloc("Cannot allocate requested " + std::to_string(sizeof(T)) + " bytes for data type "
                        + derplib::util::type_name<T>());
  }

  entry e = {ptr, sizeof(T)};
  _entries.insert(e);

  new(ptr) T(args...);
  return reinterpret_cast<T*>(ptr);
}

template<typename T, bool STRICT_CHECK>
T* mem_alloc::get(void* const ptr) {
  const auto it = std::find_if(_entries.begin(),
                               _entries.end(),
                               [ptr](const entry& e) { return reinterpret_cast<const unsigned char*>(ptr) == e.ptr; });
  if (it != _entries.end()) {
    if (sizeof(T) > it->extent) {
#if !defined(NDEBUG)
      throw std::logic_error("Type conversion error: Requested target type [" + derplib::util::type_name<T>()
                                 + "] larger than allocated size (" + std::to_string(it->extent) + ")");
#else
      throw std::logic_error("Type conversion error: Requested target type smaller than allocated size");
#endif  // defined(NDEBUG)
    }

    if DERPLIB_CPP17_CONSTEXPR (STRICT_CHECK) {
      if (sizeof(T) < it->extent) {
#if !defined(NDEBUG)
        throw std::logic_error("Type conversion error: Requested target type [" + derplib::util::type_name<T>()
                                   + "] smaller than allocated size (" + std::to_string(it->extent) + ")");
#else
        throw std::logic_error("Type conversion error: Requested target type smaller than allocated size");
#endif  // defined(NDEBUG)
      }
    }

    return reinterpret_cast<T*>(ptr);
  } else {
    return nullptr;
  }
}

template<typename T, bool STRICT_CHECK>
T* mem_alloc::get(const std::ptrdiff_t offset) {
  return get < T, STRICT_CHECK > (_heap_pool.get() + offset);
}

template<typename T>
void mem_alloc::deallocate(T*& ptr) {
  ptr->~T();

  const auto it = std::find_if(_entries.begin(),
                               _entries.end(),
                               [ptr](const entry& e) { return reinterpret_cast<unsigned char*>(ptr) == e.ptr; });
  if (it != _entries.end()) {
    _entries.erase(it);
  }
}

}  // namespace experimental
}  // namespace derplib

#include "derplib/internal/common_macros_end.h"
