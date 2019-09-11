#include "derplib/experimental/mem_alloc.h"

#include <cassert>

namespace derplib {
namespace experimental {

mem_alloc::mem_alloc(std::size_t size)
    : _size(size), _heap_pool(derplib::stdext::make_unique<unsigned char[]>(size)) {
  std::cout << "Allocated memory range: " << reinterpret_cast<void*>(_heap_pool.get()) << "\t"
            << reinterpret_cast<void*>(_heap_pool.get() + _size) << std::endl;
}

void mem_alloc::heap_dump(std::ostream& os) noexcept {
  using ptr_t = const void*;

  const unsigned char* current_ptr = _heap_pool.get();
  const unsigned char* const end_ptr = _heap_pool.get() + _size;
  auto entry_it = _entries.begin();

  while (current_ptr != end_ptr) {
    if (entry_it == _entries.end()) {
      os << std::hex << reinterpret_cast<ptr_t>(current_ptr) << "\t"
                << reinterpret_cast<ptr_t>(end_ptr) << " BLANK\t\t"
                << std::dec << end_ptr - current_ptr << " BYTES\n";
      break;
    } else if (entry_it->ptr == current_ptr) {
      os << std::hex << reinterpret_cast<ptr_t>(current_ptr) << "\t"
                << reinterpret_cast<ptr_t>(current_ptr + entry_it->extent)
                << " ALLOCATED\t" << std::dec << entry_it->extent << " BYTES\n";

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

unsigned char* mem_alloc::find_first_fit(const std::size_t size) {
  if (_entries.empty()) {
    if (size > _size) {
      return nullptr;
    }

    return _heap_pool.get();
  }

  unsigned char* extent_begin;
  unsigned char* extent_end;

  heap_walk_iterator iterator(_entries);

  extent_begin = _heap_pool.get();
  extent_end = iterator->ptr;

  // edge case 1: beginning block
  assert(extent_end >= extent_begin);
  if (std::size_t(extent_end - extent_begin) >= size) {
    return extent_begin;
  }

  // move the iterator fwd
  ++iterator;

  // nominal case: in-between blocks
  for (; iterator.current() != _entries.end(); ++iterator) {
    extent_begin = iterator.prev()->ptr + iterator.prev()->extent;
    extent_end = iterator->ptr;

    assert(extent_end >= extent_begin);
    if (std::size_t(extent_end - extent_begin) >= size) {
      return extent_begin;
    }
  }

  // edge case 2: end block
  extent_begin = iterator.prev()->ptr + iterator.prev()->extent;
  extent_end = _heap_pool.get() + _size;

  assert(extent_end >= extent_begin);
  if (std::size_t(extent_end - extent_begin) >= size) {
    return extent_begin;
  }

  return nullptr;
}

}  // namespace experimental
}  // namespace derplib
