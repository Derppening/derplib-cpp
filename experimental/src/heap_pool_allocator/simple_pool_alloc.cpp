#include "derplib/experimental/heap_pool_allocator/simple_pool_alloc.h"

#include <iomanip>
#include <iostream>

#include <derplib/stdext/memory.h>

namespace derplib {
namespace experimental {

#include <derplib/internal/common_macros_begin.h>

simple_pool_alloc::simple_pool_alloc(std::size_t n, DERPLIB_MAYBE_UNUSED const simple_pool_alloc::config& config) :
    _size(n),
    _heap_pool_(derplib::stdext::make_unique<unsigned char[]>(n)) {}

simple_pool_alloc::~simple_pool_alloc() {
  volatile unsigned char* ptr = _heap_pool_.get();
  std::fill(ptr, ptr + _size, 0);
}

void* simple_pool_alloc::allocate(std::size_t size, std::size_t alignment) noexcept {
  _entry e = {size, alignment};

  // edge case 1: empty allocation block
  if (_entries_.empty()) {
    if (size > _size) {
      return nullptr;
    }

    const auto it = _entries_.emplace(std::make_pair(_heap_pool_.get(), e));
    return it.first->first;
  }

  void* aligned_extent;
  unsigned char* extent_begin;
  unsigned char* extent_end;
  std::size_t extent_size;

  heap_entry_iterator iterator(_entries_);

  aligned_extent = extent_begin = _heap_pool_.get();
  extent_end = static_cast<unsigned char*>(iterator->first);
  extent_size = std::size_t(extent_end - extent_begin);

  // edge case 2: beginning block
  if (std::align(alignment, size, aligned_extent, extent_size) != nullptr) {
    _entries_.emplace(std::make_pair(aligned_extent, e));
    return aligned_extent;
  }

  // move the iterator fwd
  ++iterator;

  // nominal case: in-between blocks
  for (; iterator.current() != _entries_.end(); ++iterator) {
    aligned_extent = extent_begin =
        static_cast<unsigned char*>(iterator.prev()->first) + iterator.prev()->second._extent;
    extent_end = static_cast<unsigned char*>(iterator->first);
    extent_size = std::size_t(extent_end - extent_begin);

    if (std::align(alignment, size, aligned_extent, extent_size) != nullptr) {
      _entries_.emplace(std::make_pair(aligned_extent, e));
      return aligned_extent;
    }
  }

  // edge case 3: end block
  aligned_extent = extent_begin = static_cast<unsigned char*>(iterator.prev()->first) + iterator.prev()->second._extent;
  extent_end = _heap_pool_.get() + _size;
  extent_size = std::size_t(extent_end - extent_begin);

  if (std::align(alignment, size, aligned_extent, extent_size) != nullptr) {
    _entries_.emplace(std::make_pair(aligned_extent, e));
    return aligned_extent;
  }

  return nullptr;
}

void simple_pool_alloc::deallocate(void* p) noexcept {
  const auto it = _entries_.find(p);
  if (it != _entries_.end()) {
    _entries_.erase(it);
  }
}

void simple_pool_alloc::heap_dump(std::ostream& os) const noexcept {
  using ptr_t = const void*;

  const unsigned char* current_ptr = _heap_pool_.get();
  const unsigned char* const end_ptr = _heap_pool_.get() + _size;
  auto entry_it = _entries_.begin();

  os << std::setfill(' ') << std::setw(RegionTextPadding) << "REGION START"
     << "  " << std::setw(RegionTextPadding) << "REGION END"
     << "  " << std::setw(4) << "S"
     << "  " << std::setw(SizeTextPadding + 2) << "SIZE"
     << "  " << std::setw(AlignTextPadding + 2) << "ALIGN" << '\n';

  while (current_ptr != end_ptr) {
    if (entry_it == _entries_.end()) {
      os << std::showbase << std::setiosflags(std::ios::internal) << std::setfill('0') << std::setw(RegionTextPadding)
         << static_cast<ptr_t>(current_ptr) << "  " << std::setw(RegionTextPadding) << static_cast<ptr_t>(end_ptr)
         << "  Free  " << std::setfill(' ') << std::setw(SizeTextPadding) << end_ptr - current_ptr << " B\n";
      break;
    } else if (entry_it->first == current_ptr) {
      os << std::showbase << std::setiosflags(std::ios::internal) << std::setfill('0') << std::setw(RegionTextPadding)
         << static_cast<ptr_t>(current_ptr) << "  " << std::setw(RegionTextPadding)
         << static_cast<ptr_t>(current_ptr + entry_it->second._extent) << "  Used  " << std::setfill(' ')
         << std::setw(SizeTextPadding) << entry_it->second._extent << " B  " << std::setw(AlignTextPadding)
         << entry_it->second._alignment << " B  " << '\n';

      current_ptr = static_cast<unsigned char*>(entry_it->first) + entry_it->second._extent;
    } else {
      ++entry_it;

      std::ptrdiff_t blank_space = static_cast<unsigned char*>(entry_it->first) - current_ptr;

      if (entry_it != _entries_.end() && blank_space != 0) {
        os << std::showbase << std::setiosflags(std::ios::internal) << std::setfill('0') << std::setw(RegionTextPadding)
           << static_cast<ptr_t>(current_ptr) << "  " << std::setw(RegionTextPadding)
           << static_cast<ptr_t>(entry_it->first) << "  Free  " << std::setfill(' ') << std::setw(SizeTextPadding)
           << blank_space << " B\n";

        current_ptr = static_cast<unsigned char*>(entry_it->first);
      }
    }
  }
}

std::size_t simple_pool_alloc::max_size() const noexcept {
  return _size;
}

#include <derplib/internal/common_macros_end.h>

}  // namespace experimental
}  // namespace derplib
