#include "derplib/experimental/mem_alloc/fixed_pool_mem_alloc.h"

#include <cassert>
#include <iomanip>

namespace derplib {
namespace experimental {

fixed_pool_mem_alloc::fixed_pool_mem_alloc(std::size_t size) :
    _size(size),
    _heap_pool(derplib::stdext::make_unique<unsigned char[]>(size)) {}

fixed_pool_mem_alloc::~fixed_pool_mem_alloc() {
  std::for_each(_entries.begin(), _entries.end(), [](const entry& e) { e.destructor(e.ptr); });
}

void fixed_pool_mem_alloc::heap_dump(std::ostream& os) noexcept {
  using ptr_t = const void*;

  const unsigned char* current_ptr = _heap_pool.get();
  const unsigned char* const end_ptr = _heap_pool.get() + _size;
  auto entry_it = _entries.begin();

  os << std::setfill(' ') << std::setw(region_text_padding) << "REGION START"
     << "  " << std::setw(region_text_padding) << "REGION END"
     << "  " << std::setw(4) << "S"
     << "  " << std::setw(size_text_padding + 2) << "SIZE"
     << "  " << std::setw(align_text_padding + 2) << "ALIGN"
     << "  "
     << "TYPENAME\n";

  while (current_ptr != end_ptr) {
    if (entry_it == _entries.end()) {
      os << std::showbase << std::setiosflags(std::ios::internal) << std::setfill('0') << std::setw(region_text_padding)
         << static_cast<ptr_t>(current_ptr) << "  " << std::setw(region_text_padding) << static_cast<ptr_t>(end_ptr)
         << "  Free  " << std::setfill(' ') << std::setw(size_text_padding) << end_ptr - current_ptr << " B\n";
      break;
    } else if (entry_it->ptr == current_ptr) {
      os << std::showbase << std::setiosflags(std::ios::internal) << std::setfill('0') << std::setw(region_text_padding)
         << static_cast<ptr_t>(current_ptr) << "  " << std::setw(region_text_padding)
         << static_cast<ptr_t>(current_ptr + entry_it->extent) << "  Used  " << std::setfill(' ')
         << std::setw(size_text_padding) << entry_it->extent << " B  " << std::setw(align_text_padding)
         << entry_it->alignment << " B  "
#if !defined(NDEBUG)
         << entry_it->type_name
#else
         << "no info"
#endif  // !defined(NDEBUG)
         << '\n';

      current_ptr = entry_it->ptr + entry_it->extent;
    } else {
      ++entry_it;

      std::ptrdiff_t blank_space = entry_it->ptr - current_ptr;

      if (entry_it != _entries.end() && blank_space != 0) {
        os << std::showbase << std::setiosflags(std::ios::internal) << std::setfill('0')
           << std::setw(region_text_padding) << static_cast<ptr_t>(current_ptr) << "  "
           << std::setw(region_text_padding) << static_cast<ptr_t>(entry_it->ptr) << "  Free  " << std::setfill(' ')
           << std::setw(size_text_padding) << blank_space << " B\n";

        current_ptr = entry_it->ptr;
      }
    }
  }
}

unsigned char* fixed_pool_mem_alloc::find_first_fit(const std::size_t size, const std::size_t alignment) {
  // edge case 1: empty allocation block
  if (_entries.empty()) {
    if (size > _size) {
      return nullptr;
    }

    return _heap_pool.get();
  }

  void* aligned_extent;
  unsigned char* extent_begin;
  unsigned char* extent_end;
  std::size_t extent_size;

  heap_entry_iterator iterator(_entries);

  aligned_extent = extent_begin = _heap_pool.get();
  extent_end = iterator->ptr;
  extent_size = std::size_t(extent_end - extent_begin);

  // edge case 2: beginning block
  assert(extent_end >= extent_begin);
  if (std::align(alignment, size, aligned_extent, extent_size) != nullptr) {
    return reinterpret_cast<unsigned char*>(aligned_extent);
  }

  // move the iterator fwd
  ++iterator;

  // nominal case: in-between blocks
  for (; iterator.current() != _entries.end(); ++iterator) {
    aligned_extent = extent_begin = iterator.prev()->ptr + iterator.prev()->extent;
    extent_end = iterator->ptr;
    extent_size = std::size_t(extent_end - extent_begin);

    assert(extent_end >= extent_begin);
    if (std::align(alignment, size, aligned_extent, extent_size) != nullptr) {
      return reinterpret_cast<unsigned char*>(aligned_extent);
    }
  }

  // edge case 3: end block
  aligned_extent = extent_begin = iterator.prev()->ptr + iterator.prev()->extent;
  extent_end = _heap_pool.get() + _size;
  extent_size = std::size_t(extent_end - extent_begin);

  assert(extent_end >= extent_begin);
  if (std::align(alignment, size, aligned_extent, extent_size) != nullptr) {
    return reinterpret_cast<unsigned char*>(aligned_extent);
  }

  return nullptr;
}

}  // namespace experimental
}  // namespace derplib
