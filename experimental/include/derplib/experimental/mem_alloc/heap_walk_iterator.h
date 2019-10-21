#pragma once

#include <set>

namespace derplib {
namespace experimental {

/**
 * \brief Iterator which can be used to walk through the object heap. Satisfies `LegacyForwardIterator` requirement.
 *
 * \tparam T Element type.
 * \tparam Comp Comparator.
 */
template<typename T, typename Comp>
class heap_walk_iterator {
 public:
  using container_type = std::set<T, Comp>;
  using base_iterator_type = typename container_type::iterator;

  using value_type = typename base_iterator_type::value_type;
  using difference_type = typename base_iterator_type::difference_type;
  using pointer = typename base_iterator_type::pointer;
  using reference = typename base_iterator_type::reference;
  using iterator_category = std::forward_iterator_tag;

  heap_walk_iterator() = default;

  /**
   * \brief Constructs an iterator pointing to the beginning of the container.
   *
   * \param container backing container of the iterator
   */
  explicit heap_walk_iterator(const container_type& container) noexcept :
      _prev_(container.begin()),
      _curr_(container.begin()) {}
  /**
   * \brief Constructs an iterator pointing to a given element of the container.
   *
   * \param container backing container of the iterator
   * \param iterator current iterator position
   */
  heap_walk_iterator(const container_type& container, base_iterator_type iterator) noexcept :
      _prev_(iterator != container.begin() ? --base_iterator_type(iterator) : container.begin()),
      _curr_(iterator) {}

  /**
   * \return Iterator to the previous entry, or `current()` if `current()` is equal to the backing container's first
   * iterator.
   */
  base_iterator_type prev() const noexcept { return _prev_; }
  /**
   * \return Iterator to the current entry.
   */
  base_iterator_type current() const noexcept { return _curr_; }

  const typename container_type::key_type& operator*() const noexcept { return *_curr_; }
  const typename container_type::key_type* operator->() const noexcept { return std::addressof(*_curr_); }

  heap_walk_iterator& operator++() noexcept {
    _prev_ = _curr_;
    ++_curr_;
    return *this;
  }
  heap_walk_iterator operator++(int) noexcept {
    auto tmp = *this;
    ++*this;
    return tmp;
  }

  bool operator==(const heap_walk_iterator& other) const noexcept {
    return _prev_ == other._prev_ && _curr_ == other._curr_;
  }

 private:
  base_iterator_type _prev_;
  base_iterator_type _curr_;
};

}  // namespace experimental
}  // namespace derplib
