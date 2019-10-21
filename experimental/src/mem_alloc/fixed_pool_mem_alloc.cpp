#include "derplib/experimental/mem_alloc/fixed_pool_mem_alloc.h"

#include <cassert>
#include <iomanip>

namespace derplib {
namespace experimental {

fixed_pool_mem_alloc::fixed_pool_mem_alloc(std::size_t size) :
    _size(size),
    _heap_pool_(derplib::stdext::make_unique<unsigned char[]>(size)) {}

fixed_pool_mem_alloc::~fixed_pool_mem_alloc() {
  std::for_each(_entries_.begin(), _entries_.end(), [](const _entry& e) { e._destructor(e._ptr); });
}

void fixed_pool_mem_alloc::heap_dump(std::ostream& os) noexcept {
  using ptr_t = const void*;

  const unsigned char* current_ptr = _heap_pool_.get();
  const unsigned char* const end_ptr = _heap_pool_.get() + _size;
  auto entry_it = _entries_.begin();

  os << std::setfill(' ') << std::setw(RegionTextPadding) << "REGION START"
     << "  " << std::setw(RegionTextPadding) << "REGION END"
     << "  " << std::setw(4) << "S"
     << "  " << std::setw(SizeTextPadding + 2) << "SIZE"
     << "  " << std::setw(AlignTextPadding + 2) << "ALIGN"
     << "  "
     << "TYPENAME\n";

  while (current_ptr != end_ptr) {
    if (entry_it == _entries_.end()) {
      os << std::showbase << std::setiosflags(std::ios::internal) << std::setfill('0') << std::setw(RegionTextPadding)
         << static_cast<ptr_t>(current_ptr) << "  " << std::setw(RegionTextPadding) << static_cast<ptr_t>(end_ptr)
         << "  Free  " << std::setfill(' ') << std::setw(SizeTextPadding) << end_ptr - current_ptr << " B\n";
      break;
    } else if (entry_it->_ptr == current_ptr) {
      os << std::showbase << std::setiosflags(std::ios::internal) << std::setfill('0') << std::setw(RegionTextPadding)
         << static_cast<ptr_t>(current_ptr) << "  " << std::setw(RegionTextPadding)
         << static_cast<ptr_t>(current_ptr + entry_it->_extent) << "  Used  " << std::setfill(' ')
         << std::setw(SizeTextPadding) << entry_it->_extent << " B  " << std::setw(AlignTextPadding)
         << entry_it->_alignment << " B  "
#if !defined(NDEBUG)
         << entry_it->_type_name
#else
         << "no info"
#endif  // !defined(NDEBUG)
         << '\n';

      current_ptr = entry_it->_ptr + entry_it->_extent;
    } else {
      ++entry_it;

      std::ptrdiff_t blank_space = entry_it->_ptr - current_ptr;

      if (entry_it != _entries_.end() && blank_space != 0) {
        os << std::showbase << std::setiosflags(std::ios::internal) << std::setfill('0') << std::setw(RegionTextPadding)
           << static_cast<ptr_t>(current_ptr) << "  " << std::setw(RegionTextPadding)
           << static_cast<ptr_t>(entry_it->_ptr) << "  Free  " << std::setfill(' ') << std::setw(SizeTextPadding)
           << blank_space << " B\n";

        current_ptr = entry_it->_ptr;
      }
    }
  }
}

unsigned char* fixed_pool_mem_alloc::_find_first_fit(size_t size, size_t alignment) {
  // edge case 1: empty allocation block
  if (_entries_.empty()) {
    if (size > _size) {
      return nullptr;
    }

    return _heap_pool_.get();
  }

  void* aligned_extent;
  unsigned char* extent_begin;
  unsigned char* extent_end;
  std::size_t extent_size;

  heap_entry_iterator iterator(_entries_);

  aligned_extent = extent_begin = _heap_pool_.get();
  extent_end = iterator->_ptr;
  extent_size = std::size_t(extent_end - extent_begin);

  // edge case 2: beginning block
  assert(extent_end >= extent_begin);
  if (std::align(alignment, size, aligned_extent, extent_size) != nullptr) {
    return reinterpret_cast<unsigned char*>(aligned_extent);
  }

  // move the iterator fwd
  ++iterator;

  // nominal case: in-between blocks
  for (; iterator.current() != _entries_.end(); ++iterator) {
    aligned_extent = extent_begin = iterator.prev()->_ptr + iterator.prev()->_extent;
    extent_end = iterator->_ptr;
    extent_size = std::size_t(extent_end - extent_begin);

    assert(extent_end >= extent_begin);
    if (std::align(alignment, size, aligned_extent, extent_size) != nullptr) {
      return reinterpret_cast<unsigned char*>(aligned_extent);
    }
  }

  // edge case 3: end block
  aligned_extent = extent_begin = iterator.prev()->_ptr + iterator.prev()->_extent;
  extent_end = _heap_pool_.get() + _size;
  extent_size = std::size_t(extent_end - extent_begin);

  assert(extent_end >= extent_begin);
  if (std::align(alignment, size, aligned_extent, extent_size) != nullptr) {
    return reinterpret_cast<unsigned char*>(aligned_extent);
  }

  return nullptr;
}

}  // namespace experimental
}  // namespace derplib
