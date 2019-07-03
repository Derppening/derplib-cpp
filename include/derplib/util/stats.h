// Utilities for displaying memory usage for STL containers.

#pragma once

#include <deque>
#include <forward_list>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>

#include "derplib/stdext/version.h"

namespace derplib {
namespace util {

template<typename T>
void container_mem(const T& obj);

template<class T, std::size_t N>
void container_mem(const std::array<T, N>& obj);

template<typename T, typename Allocator>
void container_mem(const std::deque<T, Allocator>& obj);

template<typename CharT, typename Traits, typename Allocator>
void container_mem(const std::basic_string<CharT, Traits, Allocator>& str);

template<typename T, typename Allocator>
void container_mem(const std::forward_list<T, Allocator>& obj);

template<typename T, typename Allocator>
void container_mem(const std::list<T, Allocator>& obj);

template<typename Key, typename T, typename Compare, typename Allocator>
void container_mem(const std::map<Key, T, Compare, Allocator>& obj);

template<typename Key, typename T, typename Compare, typename Allocator>
void container_mem(const std::multimap<Key, T, Compare, Allocator>& obj);

template<typename Key, typename Compare, typename Allocator>
void container_mem(const std::multiset<Key, Compare, Allocator>& obj);

#if defined(DERPLIB_HAS_LIB_OPTIONAL)
template<typename T>
void container_mem(const std::optional<T>& obj);
#endif  // defined(DERPLIB_HAS_LIB_OPTIONAL)

template<typename T, typename Container, typename Compare>
void container_mem(const std::priority_queue<T, Container, Compare>& obj);

template<typename T, typename Container>
void container_mem(const std::queue<T, Container>& obj);

template<typename T, typename Container>
void container_mem(const std::stack<T, Container>& obj);

template<typename Key, typename Compare, typename Allocator>
void container_mem(const std::set<Key, Compare, Allocator>& obj);

template<typename T>
void container_mem(const std::shared_ptr<T>& obj);

template<typename T, typename Deleter>
void container_mem(const std::unique_ptr<T, Deleter>& obj);

template<typename Key, typename T, typename Hash, typename KeyEqual, typename Allocator>
void container_mem(const std::unordered_map<Key, T, Hash, KeyEqual, Allocator>& obj);

template<typename Key, typename T, typename Hash, typename KeyEqual, typename Allocator>
void container_mem(const std::unordered_multimap<Key, T, Hash, KeyEqual, Allocator>& obj);

template<typename Key, typename Hash, typename KeyEqual, typename Allocator>
void container_mem(const std::unordered_set<Key, Hash, KeyEqual, Allocator>& obj);

template<typename Key, typename Hash, typename KeyEqual, typename Allocator>
void container_mem(const std::unordered_multiset<Key, Hash, KeyEqual, Allocator>& obj);

template<typename T, typename Allocator>
void container_mem(const std::vector<T, Allocator>& obj);

}  // namespace util
}  // namespace derplib

#include "derplib/util/stats.ipp"
