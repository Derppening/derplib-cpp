#pragma once

#include <algorithm>
#include <iostream>
#include <iterator>
#include <memory>
#include <set>
#include <stdexcept>

#include <derplib/stdext/memory.h>
#include <derplib/util/demangle.h>

#include "bad_alloc.h"
#include "heap_walk_iterator.h"

namespace derplib {
namespace experimental {

/**
 * \brief A fixed-size pool heap memory allocator.
 */
class fixed_pool_mem_alloc final {
 private:
  struct entry;
  struct entry_set_comparator;

 public:
  /**
   * \brief Constructs an allocator.
   * 
   * \param size size of the heap
   */
  explicit fixed_pool_mem_alloc(std::size_t size);

  /**
   * Destructs all allocated objects and frees the allocated memory.
   */
  ~fixed_pool_mem_alloc();

  /**
   * \brief Allocates an object onto the heap using this allocator.
   *
   * \tparam T type of object to allocate
   * \tparam Args type of arguments
   * \param args arguments to pass to the constructor of this object
   * \return Pointer to the newly created object.
   * \throws mem_alloc::bad_alloc if the allocation cannot be done.
   */
  template<typename T, typename... Args>
  T* allocate(Args&& ... args);

  /**
   * \brief Retrieves an object from the heap.
   *
   * \tparam T type of object
   * \tparam STRICT_CHECK if true, checks whether the size of the object is exactly as big as `T`
   * \param ptr pointer to the location of the heap allocated object
   * \return `(T) ptr` if the object is found, otherwise `nullptr`
   * \throws std::logic_error if the object does not fit into the requested type `T`.
   */
  template<typename T, bool STRICT_CHECK = false>
  T* get(void* ptr);

  /**
   * \brief Retrieves an object from the heap.
   *
   * \tparam T type of object
   * \tparam STRICT_CHECK if true, checks whether the size of the object is exactly as big as `T`
   * \param offset offset of the object from the start of the heap
   * \return pointer to the object if the object is found, otherwise `nullptr`
   * \throws std::logic_error if the object does not fit into the requested type `T`.
   */
  template<typename T, bool STRICT_CHECK = false>
  T* get(std::ptrdiff_t offset);

  /**
   * \brief Deallocates the space occupied by the pointer.
   *
   * \tparam T type of object
   * \param ptr pointer to the object to be deallocated
   */
  template<typename T>
  void deallocate(T*& ptr);

  /**
   * \brief Dumps the memory allocation information to a stream.
   *
   * \param os `std::ostream` to dump the contents to
   */
  void heap_dump(std::ostream& os = std::cout) noexcept;

 private:
  using heap_entry_iterator = heap_walk_iterator<entry, entry_set_comparator>;

  static constexpr const int region_text_padding = 18;
  static constexpr const int size_text_padding = 12;
  static constexpr const int align_text_padding = 4;

  /**
   * \brief Entry of an object in the heap.
   */
  struct entry {
    using destructor_type = void(*)(const void*);

    /**
     * \brief Pointer to the first byte in the heap.
     */
    unsigned char* ptr;
    /**
     * \brief Destruction method.
     */
    const destructor_type destructor;
    /**
     * \brief Extent of the allocation in bytes.
     */
    const std::size_t extent;
    /**
     * \brief Padding of the allocation in bytes.
     */
    const std::size_t alignment;
    /**
     * \brief Demangled name of type.
     */
    const std::string type_name;
  };

  /**
   * \brief Custom comparator for `std::set<entry>`.
   *
   * Sorts the set by the pointer locations.
   */
  struct entry_set_comparator {
    bool operator()(const entry& lhs, const entry& rhs) const {
      return lhs.ptr < rhs.ptr;
    }
  };

  /**
   * \brief Finds a memory segment which can fit a given size allocation.
   *
   * \param size size of allocation to fit
   * \param alignment alignment of the allocation
   * \return Pointer to the heap if a segment is available, otherwise `nullptr`.
   */
  unsigned char* find_first_fit(std::size_t size, std::size_t alignment);

  const std::size_t _size;
  std::unique_ptr<unsigned char[]> _heap_pool;
  std::set<entry, entry_set_comparator> _entries;
};

}  // namespace experimental
}  // namespace derplib

#include "fixed_pool_mem_alloc.ipp"
