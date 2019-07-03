#include <array>
#include <deque>
#include <map>
#include <vector>

namespace derplib {
namespace newlib {

template<typename T, std::size_t N>
T* element_at(std::array<T, N>& a, typename std::array<T, N>::size_type pos) noexcept {
  if (pos < a.size()) {
    return &a[pos];
  }

  return nullptr;
}

template<typename T, std::size_t N>
const T* element_at(const std::array<T, N>& v, typename std::array<T, N>::size_type pos) noexcept {
  if (pos < v.size()) {
    return &v[pos];
  }

  return nullptr;
}

template<typename T, typename Allocator>
T* element_at(std::vector<T, Allocator>& v, typename std::vector<T, Allocator>::size_type pos) noexcept {
  if (pos < v.size()) {
    return &v[pos];
  }

  return nullptr;
}

template<typename T, typename Allocator>
const T* element_at(const std::vector<T, Allocator>& v, typename std::vector<T, Allocator>::size_type pos) noexcept {
  if (pos < v.size()) {
    return &v[pos];
  }

  return nullptr;
}

template<typename T, typename Allocator>
T* element_at(std::deque<T, Allocator>& v, typename std::deque<T, Allocator>::size_type pos) noexcept {
  if (pos < v.size()) {
    return &v[pos];
  }

  return nullptr;
}

template<typename T, typename Allocator>
const T* element_at(const std::deque<T, Allocator>& v, typename std::deque<T, Allocator>::size_type pos) noexcept {
  if (pos < v.size()) {
    return &v[pos];
  }

  return nullptr;
}

template<typename Key, typename T, typename Compare, typename Allocator>
T* element_at(std::map<Key, T, Compare, Allocator>& m, const Key& key) noexcept {
  if (m.find(key) != m.end()) {
    return &m[key];
  }

  return nullptr;
}

template<typename Key, typename T, typename Compare, typename Allocator>
const T* element_at(const std::map<Key, T, Compare, Allocator>& m, const Key& key) noexcept {
  if (m.find(key) != m.cend()) {
    return &m[key];
  }

  return nullptr;
}

}  // namespace newlib
}  // namespace derplib
