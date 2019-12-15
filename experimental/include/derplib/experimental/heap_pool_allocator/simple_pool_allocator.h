#pragma once

#include <cstdint>
#include <map>
#include <memory>

namespace derplib {
namespace experimental {

#include <derplib/internal/common_macros_begin.h>

/**
 * \brief A simple pool allocator.
 *
 * This allocator allocates a fixed-size contiguous region of memory as the heap memory pool. Fulfills the `Allocator`
 * named requirement.
 */
class simple_pool_allocator final {
 private:
  struct _entry;

 public:
  using value_type = void;

  /**
   * \brief Allocator configuration.
   */
  struct config {
    /**
     * \brief Whether to zero out the allocated region when deallocating.
     */
    bool zero_memory_after_free = false;
    /**
     * \brief Whether to zero out the entire memory pool when the allocator is destructed.
     */
    bool zero_memory_on_destruct = true;
  };

  /**
   * \brief Constructs the allocator with a heap pool.
   *
   * \param n size of the heap
   * \param config configuration of this allocator
   */
  simple_pool_allocator(std::size_t n, const config& config);
  ~simple_pool_allocator();

  simple_pool_allocator(const simple_pool_allocator&) = delete;
  simple_pool_allocator(simple_pool_allocator&&) noexcept = default;

  simple_pool_allocator& operator=(const simple_pool_allocator&) & = delete;
  simple_pool_allocator& operator=(simple_pool_allocator&&) & noexcept = delete;

  /**
   * \brief Allocates a region of memory from the heap.
   *
   * \param n size of the allocation
   * \return Pointer to the region with the required size, or `nullptr` if no such region was found.
   *
   * \note This function cannot allocate an aligned region of memory. If such functionality is required, use the
   * `allocate(std::size_t,std::size_t)` overload.
   */
  void* allocate(std::size_t n) noexcept;
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
  /**
   * \brief Deallocates a region of memory from the heap.
   *
   * \param p pointer to the object allocated by this heap pool
   * \param n size of the object, or `0` if the size is unknown
   *
   * \note This function exists to satisfy the `Allocator` named requirement. The `deallocate(void*)` overload is
   * generally preferred over this overload.
   */
  void deallocate(void* p, std::size_t n) noexcept;

  void heap_dump(std::ostream& os) const noexcept;
  DERPLIB_NODISCARD std::size_t max_size() const noexcept;

 private:
  static constexpr const int RegionTextPadding = 18;
  static constexpr const int SizeTextPadding = 12;
  static constexpr const int AlignTextPadding = 4;

  struct _entry {
    const std::size_t _extent;
    const std::size_t _alignment;
  };

  /**
   * \brief Tries to allocate a region of memory between the start of the memory pool and the first allocated region.
   *
   * \param e properties of the allocation
   * \return The memory address to the allocated region, or `nullptr` of the allocation cannot be made.
   */
  void* try_alloc_begin(_entry e) noexcept;
  /**
   * \brief Tries to allocate a region of memory between allocated regions.
   *
   * \param e properties of the allocation
   * \return The memory address to the allocated region, or `nullptr` of the allocation cannot be made.
   */
  void* try_alloc_nominal(_entry e) noexcept;
  /**
   * \brief Tries to allocate a region of memory between the last allocated region and the end of the memory pool.
   *
   * \param e properties of the allocation
   * \return The memory address to the allocated region, or `nullptr` of the allocation cannot be made.
   */
  void* try_alloc_end(_entry e) noexcept;

  const config _config_;

  const std::size_t _size;
  std::unique_ptr<unsigned char[]> _heap_pool_;
  std::map<void*, _entry> _entries_;

  /**
   * \brief The lower and upper bound for the region where allocations are possible.
   *
   * To avoid checking regions which do not contain space for new allocations, we cache boundaries between the first and
   * last empty regions. This results in the beginning and end contiguous allocation entries to be skipped during free
   * space checking, and can improve performance on non-fragmented heaps.
   */
  std::pair<void*, void*> _alloc_bounds_;
};

#include <derplib/internal/common_macros_end.h>

}  // namespace experimental
}  // namespace derplib
