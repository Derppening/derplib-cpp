#include "derplib/experimental/heap_pool_allocator/simple_pool_allocator.h"

#include <iomanip>
#include <iostream>

#include <derplib/stdext/memory.h>

namespace derplib {
namespace experimental {

#include <derplib/internal/common_macros_begin.h>

simple_pool_allocator::simple_pool_allocator(std::size_t n, const simple_pool_allocator::config& config) :
    _config_{config},
    _size{n},
    _heap_pool_{derplib::stdext::make_unique<unsigned char[]>(n)},
    _alloc_bounds_{_heap_pool_.get(), _heap_pool_.get() + _size} {}

simple_pool_allocator::~simple_pool_allocator() {
  if (_config_.zero_memory_on_destruct) {
    if (_heap_pool_) {
      volatile unsigned char* ptr{_heap_pool_.get()};
      std::fill(ptr, ptr + _size, 0);
    }
  }
}

void* simple_pool_allocator::allocate(std::size_t n) noexcept {
  return allocate(n, 1);
}

void* simple_pool_allocator::allocate(std::size_t size, std::size_t alignment) noexcept {
  const _entry e{size, alignment};
  void* alloc_ptr{nullptr};

  // fast throw if we can't fit the allocation
  if (e._extent > _size) {
    return nullptr;
  }
  // fast throw if we are out of memory
  if (_alloc_bounds_.first > _alloc_bounds_.second) {
    return nullptr;
  }

  if ((alloc_ptr = try_alloc_begin(e)) != nullptr) {
    return alloc_ptr;
  }
  if ((alloc_ptr = try_alloc_nominal(e)) != nullptr) {
    return alloc_ptr;
  }
  if ((alloc_ptr = try_alloc_end(e)) != nullptr) {
    return alloc_ptr;
  }

  return nullptr;
}

void simple_pool_allocator::deallocate(void* p) noexcept {
  deallocate(p, 0);
}

void simple_pool_allocator::deallocate(void* p, std::size_t n) noexcept {
  const auto it{_entries_.find(p)};
  if (it != _entries_.end()) {
    if (n != 0 && it->second._extent != n) {
      return;
    }

    void* const ptr{it->first};

    if (_config_.zero_memory_after_free) {
      volatile auto* const begin{static_cast<unsigned char*>(ptr)};
      volatile auto* const end{begin + it->second._extent};

      std::fill(begin, end, 0);
    }

    auto next_it{_entries_.erase(it)};
    if (_alloc_bounds_.second < _alloc_bounds_.first) {
      _alloc_bounds_.first = ptr;
      _alloc_bounds_.second = next_it == _entries_.end() ? _heap_pool_.get() + _size : next_it->first;
    } else {
      if (ptr <= _alloc_bounds_.first) {
        _alloc_bounds_.first = next_it == _entries_.begin() ? _heap_pool_.get() : (--next_it)->first;
      } else if (ptr >= _alloc_bounds_.second) {
        _alloc_bounds_.second = next_it == _entries_.end() ? _heap_pool_.get() + _size : next_it->first;
      }
    }
  }
}

void simple_pool_allocator::heap_dump(std::ostream& os) const noexcept {
  using ptr_t = const void*;

  const unsigned char* current_ptr{_heap_pool_.get()};
  const unsigned char* const end_ptr{_heap_pool_.get() + _size};
  auto entry_it{_entries_.begin()};

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

      // TODO: const
      std::ptrdiff_t blank_space{static_cast<unsigned char*>(entry_it->first) - current_ptr};

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

std::size_t simple_pool_allocator::max_size() const noexcept {
  return _size;
}

void* simple_pool_allocator::try_alloc_begin(const _entry e) noexcept {
  if (_alloc_bounds_.first > _heap_pool_.get()) {
    return nullptr;
  }

  void* aligned_extent{_heap_pool_.get()};
  const auto* const extent_begin{static_cast<unsigned char*>(aligned_extent)};
  const auto* const extent_end{static_cast<unsigned char*>(_entries_.begin()->first)};
  auto extent_size{static_cast<std::size_t>(extent_end - extent_begin)};

  if (std::align(e._alignment, e._extent, aligned_extent, extent_size) != nullptr) {
    _entries_.emplace(std::make_pair(aligned_extent, e));

    auto* const alloc_end{static_cast<unsigned char*>(aligned_extent) + e._extent};
    if (_alloc_bounds_.first < alloc_end) {
      _alloc_bounds_.first = alloc_end;
    }
    if (_alloc_bounds_.second == alloc_end) {
      _alloc_bounds_.second = aligned_extent;
    }

    return aligned_extent;
  }

  return nullptr;
}

void* simple_pool_allocator::try_alloc_nominal(simple_pool_allocator::_entry e) noexcept {
  const auto lower_bound{_entries_.lower_bound(_alloc_bounds_.first)};
  const auto upper_bound{_entries_.upper_bound(_alloc_bounds_.second)};

  if (lower_bound == upper_bound) {
    return nullptr;
  }

  heap_entry_iterator iterator{_entries_, lower_bound};

  for (; iterator.current() != upper_bound; ++iterator) {
    const auto last_it{iterator.prev()};

    void* aligned_extent{static_cast<unsigned char*>(last_it->first) + last_it->second._extent};
    const auto* const extent_begin{static_cast<unsigned char*>(aligned_extent)};
    const auto* const extent_end{static_cast<unsigned char*>(iterator->first)};
    auto extent_size{static_cast<std::size_t>(extent_end - extent_begin)};

    if (std::align(e._alignment, e._extent, aligned_extent, extent_size) != nullptr) {
      _entries_.emplace(std::make_pair(aligned_extent, e));

      auto* const alloc_end{static_cast<unsigned char*>(aligned_extent) + e._extent};
      if (_alloc_bounds_.first < alloc_end) {
        _alloc_bounds_.first = alloc_end;
      }
      if (_alloc_bounds_.second == alloc_end) {
        _alloc_bounds_.second = aligned_extent;
      }

      return aligned_extent;
    }
  }

  return nullptr;
}

void* simple_pool_allocator::try_alloc_end(simple_pool_allocator::_entry e) noexcept {
  if (_alloc_bounds_.second < _heap_pool_.get() + _size) {
    return nullptr;
  }

  const auto last_it{--_entries_.end()};

  void* aligned_extent{static_cast<unsigned char*>(last_it->first) + last_it->second._extent};
  const auto* const extent_begin{static_cast<unsigned char*>(aligned_extent)};
  const auto* const extent_end{_heap_pool_.get() + _size};
  auto extent_size{static_cast<std::size_t>(extent_end - extent_begin)};

  if (std::align(e._alignment, e._extent, aligned_extent, extent_size) != nullptr) {
    _entries_.emplace(std::make_pair(aligned_extent, e));

    auto* const alloc_end{static_cast<unsigned char*>(aligned_extent) + e._extent};
    if (_alloc_bounds_.first < alloc_end) {
      _alloc_bounds_.first = alloc_end;
    }
    if (_alloc_bounds_.second == alloc_end) {
      _alloc_bounds_.second = aligned_extent;
    }

    return aligned_extent;
  }

  return nullptr;
}

#include <derplib/internal/common_macros_end.h>

}  // namespace experimental
}  // namespace derplib
