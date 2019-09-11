#include "derplib/experimental/mem_alloc.h"

namespace derplib {
namespace experimental {

mem_alloc::heap_walk_iterator::heap_walk_iterator(const mem_alloc::heap_walk_iterator::container_type& container) noexcept :
    _prev(container.begin()),
    _curr(container.begin()) {}

mem_alloc::heap_walk_iterator::heap_walk_iterator(const container_type& container, const iterator_type iterator) noexcept :
    _prev(iterator != container.begin() ? --iterator_type(iterator) : container.begin()),
    _curr(iterator) {}

mem_alloc::heap_walk_iterator& mem_alloc::heap_walk_iterator::operator++() noexcept {
  _prev = _curr;
  ++_curr;
  return *this;
}

}  // namespace experimental
}  // namespace derplib
