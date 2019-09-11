#pragma once

#include <algorithm>
#include <iostream>
#include <iterator>
#include <memory>
#include <set>
#include <stdexcept>

#include <derplib/stdext/memory.h>
#include <derplib/util/demangle.h>

namespace derplib {
namespace experimental {

/**
 * \brief A memory allocator.
 *
 * TODO: Support for resizing, compacting, automatic garbage collection, etc.
 *
 * I don't know why I wanted to write this.
 */
class mem_alloc {
 public:
  /**
   * \brief Thrown when an allocation cannot be made with the current heap.
   * 
   * Similar to `std::bad_alloc`, but also provides an optional message on why the allocation failed.
   */
  class bad_alloc : public std::bad_alloc {
   public:
    bad_alloc();
    bad_alloc(const char* what);
    bad_alloc(const std::string& what);

    bad_alloc(const bad_alloc& other) = default;

    const char* what() const noexcept override;

   private:
    const char* _what;
  };

  /**
   * \brief Constructs an allocator.
   * 
   * \param size size of the heap
   */
  explicit mem_alloc(std::size_t size);

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
  /**
   * \brief Entry of an object in the heap.
   */
  struct entry {
    /**
     * \brief Pointer to the first byte in the heap.
     */
    unsigned char* ptr;
    /**
     * \brief Extent of the allocation in bytes.
     */
    std::size_t extent;
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
   * \brief Iterator which can be used to walk through the object heap.
   */
  class heap_walk_iterator {
   public:
    using container_type = std::set<entry, entry_set_comparator>;
    using iterator_type = container_type::iterator;

    heap_walk_iterator() = default;

    /**
     * \brief Constructs an iterator pointing to the beginning of the container.
     *
     * \param container backing container of the iterator
     */
    explicit heap_walk_iterator(const container_type& container) noexcept;
    /**
     * \brief Constructs an iterator pointing to a given element of the container.
     *
     * \param container backing container of the iterator
     * \param iterator current iterator position
     */
    heap_walk_iterator(const container_type& container, iterator_type iterator) noexcept;

    /**
     * \return Iterator to the previous entry, or `current()` if `current()` is equal to the backing container's first
     * iterator.
     */
    iterator_type prev() const noexcept { return _prev; }
    /**
     * \return Iterator to the current entry.
     */
    iterator_type current() const noexcept { return _curr; }

    const container_type::key_type& operator*() const noexcept { return *_curr; }
    const container_type::key_type* operator->() const noexcept { return std::addressof(*_curr); }

    heap_walk_iterator& operator++() noexcept;
    heap_walk_iterator operator++(int) noexcept {
      auto tmp = *this;
      ++*this;
      return tmp;
    }

    bool operator==(const heap_walk_iterator& other) const noexcept {
      return _prev == other._prev && _curr == other._curr;
    }

   private:
    iterator_type _prev;
    iterator_type _curr;
  };

  /**
   * \brief Finds a memory segment which can fit a given size allocation.
   *
   * \param size size of allocation to fit
   * \return Pointer to the heap if a segment is available, otherwise `nullptr`.
   */
  unsigned char* find_first_fit(std::size_t size);

  const std::size_t _size;
  std::unique_ptr<unsigned char[]> _heap_pool;
  std::set<entry, entry_set_comparator> _entries;

//  friend class std::iterator_traits<heap_walk_iterator>;
};

}  // namespace experimental
}  // namespace derplib

/*namespace std {
template<>
class iterator_traits<derplib::experimental::mem_alloc::heap_walk_iterator> {
 private:
  using base_type = derplib::experimental::mem_alloc::heap_walk_iterator;

 public:
  using value_type = base_type::iterator_type::value_type;
  using difference_type = base_type::iterator_type::difference_type;
  using pointer = base_type::iterator_type::pointer;
  using reference = base_type::iterator_type::reference;
  using iterator_category = forward_iterator_tag;
};
}  // namespace std*/

#include "mem_alloc.ipp"
