#pragma once

#include <array>
#include <deque>
#include <map>
#include <vector>

namespace derplib {
inline namespace stdext {
inline namespace newlib {

/**
 * \brief Exception-safe replacement for `std::array::at`.
 *
 * \tparam T element type of `std::array`
 * \tparam N number of elements
 * \param a array to get element from
 * \param pos position of element
 * \return Pointer to the element, or `nullptr` if `pos` is out of bounds.
 */
template<typename T, std::size_t N>
T* element_at(std::array<T, N>& a, typename std::array<T, N>::size_type pos) noexcept {
  return pos < a.size() ? &a[pos] : nullptr;
}

/**
 * \brief Exception-safe replacement for `std::array::at`.
 *
 * \tparam T element type of `std::array`
 * \tparam N number of elements
 * \param a array to get element from
 * \param pos position of element
 * \return Pointer to the element, or `nullptr` if `pos` is out of bounds.
 */
template<typename T, std::size_t N>
const T* element_at(const std::array<T, N>& a, typename std::array<T, N>::size_type pos) noexcept {
  return pos < a.size() ? &a[pos] : nullptr;
}

/**
 * \brief Exception-safe replacement for `std::vector::at`.
 *
 * \tparam T element type of `std::vector`
 * \tparam Allocator allocator type of `std::vector`
 * \param v container to get element from
 * \param pos position of element
 * \return Pointer to the element, or `nullptr` if `pos` is out of bounds.
 */
template<typename T, typename Allocator>
T* element_at(std::vector<T, Allocator>& v, typename std::vector<T, Allocator>::size_type pos) noexcept {
  return pos < v.size() ? &v[pos] : nullptr;
}

/**
 * \brief Exception-safe replacement for `std::vector::at`.
 *
 * \tparam T element type of `std::vector`
 * \tparam Allocator allocator type of `std::vector`
 * \param v container to get element from
 * \param pos position of element
 * \return Pointer to the element, or `nullptr` if `pos` is out of bounds.
 */
template<typename T, typename Allocator>
const T* element_at(const std::vector<T, Allocator>& v, typename std::vector<T, Allocator>::size_type pos) noexcept {
  return pos < v.size() ? &v[pos] : nullptr;
}

/**
 * \brief Exception-safe replacement for `std::deque::at`.
 *
 * \tparam T element type of `std::deque`
 * \tparam Allocator allocator type of `std::deque`
 * \param v container to get element from
 * \param pos position of element
 * \return Pointer to the element, or `nullptr` if `pos` is out of bounds.
 */
template<typename T, typename Allocator>
T* element_at(std::deque<T, Allocator>& v, typename std::deque<T, Allocator>::size_type pos) noexcept {
  return pos < v.size() ? &v[pos] : nullptr;
}

/**
 * \brief Exception-safe replacement for `std::deque::at`.
 *
 * \tparam T element type of `std::deque`
 * \tparam Allocator allocator type of `std::deque`
 * \param v container to get element from
 * \param pos position of element
 * \return Pointer to the element, or `nullptr` if `pos` is out of bounds.
 */
template<typename T, typename Allocator>
const T* element_at(const std::deque<T, Allocator>& v, typename std::deque<T, Allocator>::size_type pos) noexcept {
  return pos < v.size() ? &v[pos] : nullptr;
}

/**
 * \brief Exception-safe replacement for `std::map::at`.
 *
 * \tparam Key key type of `std::map`
 * \tparam T element type of `std::map`
 * \tparam Compare comparator type of `std::map`
 * \tparam Allocator allocator type of `std::map`
 * \param m container to get element from
 * \param key key of element
 * \return Pointer to the element, or `nullptr` if element does not exist
 */
template<typename Key, typename T, typename Compare, typename Allocator>
T* element_at(std::map<Key, T, Compare, Allocator>& m, const Key& key) noexcept {
  return m.find(key) != m.end() ? &m[key] : nullptr;
}

/**
 * \brief Exception-safe replacement for `std::map::at`.
 *
 * \tparam Key key type of `std::map`
 * \tparam T element type of `std::map`
 * \tparam Compare comparator type of `std::map`
 * \tparam Allocator allocator type of `std::map`
 * \param m container to get element from
 * \param key key of element
 * \return Pointer to the element, or `nullptr` if element does not exist
 */
template<typename Key, typename T, typename Compare, typename Allocator>
const T* element_at(const std::map<Key, T, Compare, Allocator>& m, const Key& key) noexcept {
  return m.find(key) != m.end() ? &m[key] : nullptr;
}

}  // namespace newlib
}  // namespace stdext
}  // namespace derplib
