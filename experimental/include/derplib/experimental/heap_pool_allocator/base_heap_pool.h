#pragma once

#include <cstdint>
#include <algorithm>
#include <iostream>
#include <map>

namespace derplib {
namespace experimental {

/**
 * \brief A heap memory pool with support for different allocation strategies.
 *
 * \tparam AllocStrategy An allocation strategy that is used to acquire/release memory in the pool.
 * \tparam ThrowIfError Whether to throw an exception if an error has occurred. Refer to the specific function for when
 * the function will throw an error.
 */
template<typename AllocStrategy, bool ThrowIfError = true>
class base_heap_pool {
 public:
  /**
   * \brief Constructs a heap memory pool.
   *
   * \param size maximum size of the heap
   * \param config optional configuration for the allocator. Refer to the allocator for configuration options.
   */
  explicit base_heap_pool(std::size_t size, const typename AllocStrategy::config& config = {});
  ~base_heap_pool();

  /**
   * \brief Allocates an object onto the heap.
   *
   * \tparam T type of object to allocate
   * \tparam Args type of constructor arguments
   * \param args arguments to pass to the constructor of this object
   * \return Pointer to the newly created object.
   * \throws std::bad_alloc if the allocation cannot be performed, and `ThrowIfError` is `true`.
   */
  template<typename T, typename... Args>
  T* allocate(Args&&... args);

  /**
   * \brief Retrieves an object from the heap.
   *
   * \tparam T type of object
   * \param ptr pointer to the object allocated by this heap pool
   * \return `(T*) ptr` if the object is found, otherwise `nullptr`
   * \throws std::invalid_argument if the object at `ptr` is not allocated by this heap pool, and `ThrowIfError` is
   * `true`.
   * \throws std::bad_cast if `T` does not match the type of the originally allocated object, and `ThrowIfError` is
   * `true`.
   */
  template<typename T>
  T* get(void* ptr);

  /**
   * \brief Deallocates the space occupied by the pointed object.
   *
   * \tparam T type of object
   * \param ptr pointer to the object to be deallocated
   * \throws std::invalid_argument if the object at `ptr` is not allocated by this heap pool, and `ThrowIfError` is
   * `true`.
   */
  template<typename T>
  void deallocate(T* ptr);

  /**
   * \brief Dumps the memory allocation information to an `std::ostream`.
   *
   * \param os `std::ostream` to dump the contents to
   */
  void heap_dump(std::ostream& os = std::cout) const noexcept;
  /**
   * \return Largest supported allocation size.
   */
  std::size_t max_size() const noexcept;

 private:
  /**
   * \brief Heap object type-dependent information.
   *
   * This POD type is used to store type-dependent information in order to enforce RAII and runtime type checking.
   */
  struct _entry {
    using destructor_type = void (*)(const void*);

    /**
     * \brief Pointer to the destructor of the object.
     */
    const destructor_type _destructor;
    /**
     * \brief Hash of the object type.
     */
    std::size_t _type_hash;
  };

  AllocStrategy _allocator_;
  std::map<void*, _entry> _entries_;
};

#include <derplib/internal/common_macros_begin.h>

template<typename AllocStrategy, bool ThrowIfError>
base_heap_pool<AllocStrategy, ThrowIfError>::base_heap_pool(std::size_t size,
                                                            const typename AllocStrategy::config& config) :
    _allocator_(size, config) {}

template<typename AllocStrategy, bool ThrowIfError>
base_heap_pool<AllocStrategy, ThrowIfError>::~base_heap_pool() {
  for (auto it = _entries_.begin(); it != _entries_.end();) {
    it->second._destructor(it->first);
    _allocator_.deallocate(it->first);
    it = _entries_.erase(it);
  }
}

template<typename AllocStrategy, bool ThrowIfError>
template<typename T, typename... Args>
T* base_heap_pool<AllocStrategy, ThrowIfError>::allocate(Args&&... args) {
  void* ptr = _allocator_.allocate(sizeof(T), alignof(T));
  if (ptr == nullptr) {
    if (ThrowIfError) {
      throw std::bad_alloc();
    } else {
      return nullptr;
    }
  }

  _entry e = {[](const void* pobj) { static_cast<const T*>(pobj)->~T(); }, typeid(T).hash_code()};
  _entries_.emplace(std::make_pair(ptr, e));

  new (ptr) T(args...);
  return reinterpret_cast<T*>(ptr);
}

template<typename AllocStrategy, bool ThrowIfError>
template<typename T>
T* base_heap_pool<AllocStrategy, ThrowIfError>::get(void* ptr) {
  const auto it = _entries_.find(ptr);
  if (it != _entries_.end()) {
    if (it->second._type_hash == typeid(T).hash_code()) {
      return reinterpret_cast<T*>(ptr);
    }

    if (ThrowIfError) {
      throw std::bad_cast();
    }

    return nullptr;
  }

  if (ThrowIfError) {
    throw std::invalid_argument("get(): Target object not allocated by this heap pool");
  }
  return nullptr;
}

template<typename AllocStrategy, bool ThrowIfError>
template<typename T>
void base_heap_pool<AllocStrategy, ThrowIfError>::deallocate(T* ptr) {
  const auto it = _entries_.find(ptr);
  if (it != _entries_.end()) {
    if (it->second._type_hash == typeid(T).hash_code()) {
      it->second._destructor(ptr);
      _allocator_.deallocate(ptr);
      _entries_.erase(it);
    } else {
      if (ThrowIfError) {
        throw std::bad_cast();
      }
    }
  } else {
    if (ThrowIfError) {
      throw std::invalid_argument("deallocate(): Target object not allocated by this heap pool");
    }
  }
}

template<typename AllocStrategy, bool ThrowIfError>
void base_heap_pool<AllocStrategy, ThrowIfError>::heap_dump(std::ostream& os) const noexcept {
  _allocator_.heap_dump(os);
}

template<typename AllocStrategy, bool ThrowIfError>
std::size_t base_heap_pool<AllocStrategy, ThrowIfError>::max_size() const noexcept {
  _allocator_.max_size();
}

#include <derplib/internal/common_macros_end.h>

}  // namespace experimental
}  // namespace derplib
