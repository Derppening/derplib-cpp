#pragma once

#include <cstdint>
#include <map>
#include <memory>

#include "derplib/experimental/heap_pool_allocator/heap_entry_iterator.h"

namespace derplib {
namespace experimental {


/**
 * \brief A simple pool allocator.
 *
 * This allocator allocates a fixed-size contiguous region of memory as the heap memory pool.
 */
class simple_pool_alloc final {
 private:
  struct _entry;

 public:
  /**
   * \brief Allocator configuration.
   */
  struct config {};

  /**
   * \brief Constructs the allocator with a heap pool.
   *
   * \param n size of the heap
   * \param config configuration of this allocator
   */
  simple_pool_alloc(std::size_t n, const config& config);
  ~simple_pool_alloc();

  /**
   * \brief Allocates a region of memory from the heap.
   *
   * \param size size of the allocation
   * \param alignment alignment of the allocation
   * \return Pointer to the region with the required size and alignment, or `nullptr` if no such region is found.
   */
  void* allocate(std::size_t size, std::size_t alignment) noexcept;
  /**
   * \brief Deallocates a region of memory from the heap.
   *
   * \param p pointer to the object allocated by this heap pool
   */
  void deallocate(void* p) noexcept;

  void heap_dump(std::ostream& os) const noexcept;
  std::size_t max_size() const noexcept;

 private:
  using heap_entry_iterator = _heap_entry_iterator<_entry>;

  static constexpr const int RegionTextPadding = 18;
  static constexpr const int SizeTextPadding = 12;
  static constexpr const int AlignTextPadding = 4;

  struct _entry {
    const std::size_t _extent;
    const std::size_t _alignment;
  };

  const std::size_t _size;
  std::unique_ptr<unsigned char[]> _heap_pool_;
  std::map<void*, _entry> _entries_;
};

}  // namespace experimental
}  // namespace derplib
