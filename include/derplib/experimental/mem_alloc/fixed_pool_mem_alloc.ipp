#include "derplib/internal/common_macros_begin.h"

namespace derplib {
namespace experimental {

template<typename T, typename... Args>
T* fixed_pool_mem_alloc::allocate(Args&& ... args) {
  constexpr std::size_t type_size = sizeof(T);
  constexpr std::size_t align_size = alignof(T);

  unsigned char* const ptr = find_first_fit(type_size, align_size);
  if (ptr == nullptr) {
    std::cerr << "Cannot allocate requested " << std::to_string(type_size)
            <<" bytes for data type "
#if !defined(NDEBUG)
            << derplib::util::type_name<T>()
#else
            << typeid(T).name()
#endif  // !defined(NDEBUG)
            << '\n';
    heap_dump(std::cerr);
    throw bad_alloc("Not enough memory for requested allocation");
  }

  entry e = {
      ptr,
      [](const void* pobj) { static_cast<const T*>(pobj)->~T(); },
      type_size,
      align_size,
#if !defined(NDEBUG)
      derplib::util::type_name<T>()
#else
      {}
#endif  // !defined(NDEBUG)
  };
  _entries.insert(e);

  new(ptr) T(args...);
  return reinterpret_cast<T*>(ptr);
}

template<typename T, bool STRICT_CHECK>
T* fixed_pool_mem_alloc::get(void* const ptr) {
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
T* fixed_pool_mem_alloc::get(const std::ptrdiff_t offset) {
  return get < T, STRICT_CHECK > (_heap_pool.get() + offset);
}

template<typename T>
void fixed_pool_mem_alloc::deallocate(T*& ptr) {
  const auto it = std::find_if(_entries.begin(),
                               _entries.end(),
                               [ptr](const entry& e) { return reinterpret_cast<unsigned char*>(ptr) == e.ptr; });
  if (it != _entries.end()) {
    it->destructor(ptr);
    _entries.erase(it);
  }
}

}  // namespace experimental
}  // namespace derplib

#include "derplib/internal/common_macros_end.h"
