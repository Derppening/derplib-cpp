#include "derplib/experimental/mem_alloc.h"

#include <cassert>

namespace derplib {
namespace experimental {

mem_alloc::mem_alloc(std::size_t size)
    : _size(size), _heap_pool(derplib::stdext::make_unique<unsigned char[]>(size)) {
  std::cout << "Allocated memory range: " << reinterpret_cast<void*>(_heap_pool.get()) << "\t"
            << reinterpret_cast<void*>(_heap_pool.get() + _size) << std::endl;
}

mem_alloc::~mem_alloc() {
  std::for_each(_entries.begin(), _entries.end(), [](const entry& e) { e.destructor(e.ptr); });
}

void mem_alloc::heap_dump(std::ostream& os) noexcept {
  using ptr_t = const void*;

  const unsigned char* current_ptr = _heap_pool.get();
  const unsigned char* const end_ptr = _heap_pool.get() + _size;
  auto entry_it = _entries.begin();

  // TODO: Redo formatting
  while (current_ptr != end_ptr) {
    if (entry_it == _entries.end()) {
      os << std::hex << reinterpret_cast<ptr_t>(current_ptr) << "\t"
                << reinterpret_cast<ptr_t>(end_ptr) << " BLANK\t\t"
                << std::dec << end_ptr - current_ptr << " BYTES\n";
      break;
    } else if (entry_it->ptr == current_ptr) {
      os << std::hex << reinterpret_cast<ptr_t>(current_ptr) << "\t"
                << reinterpret_cast<ptr_t>(current_ptr + entry_it->extent)
                << " ALLOCATED\t" << std::dec << entry_it->extent << " BYTES"
                << "\t\tALIGNED\t" << entry_it->padding << " BYTES"
#if !defined(NDEBUG)
                << "\t\t" << entry_it->type_name
#endif
                << '\n';

      current_ptr = entry_it->ptr + entry_it->extent;
    } else {
      ++entry_it;

      std::ptrdiff_t blank_space = entry_it->ptr - current_ptr;

      if (entry_it != _entries.end() && blank_space != 0) {
        os << std::hex << reinterpret_cast<ptr_t>(current_ptr) << "\t" << reinterpret_cast<ptr_t>(entry_it->ptr)
                  << " BLANK\t\t"
                  << std::dec << entry_it->ptr - current_ptr << " BYTES\n";
        current_ptr = entry_it->ptr;
      }
    }
  }
}

unsigned char* mem_alloc::find_first_fit(const std::size_t size, const std::size_t alignment) {
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

  heap_walk_iterator iterator(_entries);

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
