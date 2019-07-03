#include <deque>
#include <forward_list>
#include <iostream>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "demangle.h"

namespace derplib {
namespace util {

namespace {
template<typename T>
std::string nameof_type(const std::string& official_name = "") {
  std::string tname = type_name<T>();

  std::cout << "Type: ";
  if (!official_name.empty()) {
    std::cout << official_name << " (";
  }
  std::cout << tname;
  if (!official_name.empty()) {
    std::cout << ")\n";
  }
  return tname;
}

template<typename T>
const void* addressof_var(const T& obj) {
  const void* addr = std::addressof(obj);
  std::cout << "\tAddress: " << addr << '\n';
  return addr;
}

template<typename T>
std::size_t sizeof_type(const std::string& name = "<class>") {
  std::size_t size = sizeof(T);
  std::cout << "\tsizeof(" << name;
  if (name != "<class>") {
    std::cout << " = " << type_name<T>();
  }
  std::cout << "): " << size << " B\n";
  return size;
}
}  // namespace

template<typename T>
void container_mem(const T& obj) {
  using type = T;

  nameof_type<type>();
  addressof_var(obj);
  sizeof_type<type>();
}

template<class T, std::size_t N>
void container_mem(const std::array<T, N>& obj) {
  using type = typename std::array<T, N>;
  using value_type = typename type::value_type;

  nameof_type<type>("std::array<T, N>");
  addressof_var(obj);
  sizeof_type<type>();
  std::size_t vt_size = sizeof_type<value_type>("T");
  std::cout << "\tSize: " << obj.size() << '\n';
  std::cout << "\tUsed: " << vt_size * obj.size() << " B\n";
}

template<typename T, typename Allocator>
void container_mem(const std::deque<T, Allocator>& obj) {
  using type = typename std::deque<T, Allocator>;
  using value_type = typename type::value_type;
  using allocator_type = typename type::allocator_type;

  nameof_type<type>("std::deque<T, Allocator>");
  addressof_var(obj);
  sizeof_type<type>();
  std::size_t vt_size = sizeof_type<value_type>("T");
  sizeof_type<allocator_type>("Allocator");
  std::cout << "\tSize: " << obj.size() << '\n';
  std::cout << "\tUsed Heap: " << vt_size * obj.size() << " B\n";
}

template<typename CharT, typename Traits, typename Allocator>
void container_mem(const std::basic_string<CharT, Traits, Allocator>& str) {
  using type = typename std::basic_string<CharT, Traits, Allocator>;
  using value_type = typename type::value_type;
  using traits_type = typename type::traits_type;
  using allocator_type = typename type::allocator_type;

  nameof_type<type>("std::basic_string<CharT, Traits, Allocator>");
  addressof_var(str);
  sizeof_type<type>();
  std::size_t vt_size = sizeof_type<value_type>("CharT");
  sizeof_type<traits_type>("Traits");
  sizeof_type<allocator_type>("Allocator");
  std::cout << "\tSize: " << str.size() << '/' << str.capacity() << '\n';
  std::cout << "\tUsed Heap: " << vt_size * str.size() << '/' << vt_size * str.capacity() << " B\n";
}

template<typename T, typename Allocator>
void container_mem(const std::forward_list<T, Allocator>& obj) {
  using type = typename std::forward_list<T, Allocator>;
  using value_type = typename type::value_type;
  using allocator_type = typename type::allocator_type;

  nameof_type<type>("std::forward_list<T, Allocator>");
  addressof_var(obj);
  sizeof_type<type>();
  sizeof_type<value_type>("T");
  sizeof_type<allocator_type>("Allocator");
}

template<typename T, typename Allocator>
void container_mem(const std::list<T, Allocator>& obj) {
  using type = typename std::list<T, Allocator>;
  using value_type = typename type::value_type;
  using allocator_type = typename type::allocator_type;

  nameof_type<type>("std::list<T, Allocator>");
  addressof_var(obj);
  sizeof_type<type>();
  std::size_t vt_size = sizeof_type<value_type>("T");
  sizeof_type<allocator_type>("Allocator");
  std::cout << "\tSize: " << obj.size() << '\n';
  std::cout << "\tUsed Heap: " << vt_size * obj.size() << " B\n";
}

template<typename Key, typename T, typename Compare, typename Allocator>
void container_mem(const std::map<Key, T, Compare, Allocator>& obj) {
  using type = typename std::map<Key, T, Compare, Allocator>;
  using key_type = typename type::key_type;
  using mapped_type = typename type::mapped_type;
  using key_compare = typename type::key_compare;
  using allocator_type = typename type::allocator_type;
#if __cplusplus > 201402L
  using node_type = typename type::node_type;
#endif  // __cplusplus > 201402L

  nameof_type<type>("std::map<Key, T, Compare, Allocator>");
  addressof_var(obj);
  sizeof_type<type>();
  sizeof_type<key_type>("Key");
  sizeof_type<mapped_type>("T");
  sizeof_type<key_compare>("Compare");
  sizeof_type<allocator_type>("Allocator");
#if __cplusplus > 201402L
  sizeof_type<node_type>("node_type");
#endif  // __cplusplus > 201402L
  std::cout << "\tSize: " << obj.size() << '\n';
}

template<typename Key, typename T, typename Compare, typename Allocator>
void container_mem(const std::multimap<Key, T, Compare, Allocator>& obj) {
  using type = typename std::multimap<Key, T, Compare, Allocator>;
  using key_type = typename type::key_type;
  using mapped_type = typename type::mapped_type;
  using key_compare = typename type::key_compare;
  using allocator_type = typename type::allocator_type;
#if __cplusplus > 201402L
  using node_type = typename type::node_type;
#endif  // __cplusplus > 201402L

  nameof_type<type>("std::multimap<Key, T, Compare, Allocator>");
  addressof_var(obj);
  sizeof_type<type>();
  sizeof_type<key_type>("Key");
  sizeof_type<mapped_type>("T");
  sizeof_type<key_compare>("Compare");
  sizeof_type<allocator_type>("Allocator");
#if __cplusplus > 201402L
  sizeof_type<node_type>("node_type");
#endif  // __cplusplus > 201402L
  std::cout << "\tSize: " << obj.size() << '\n';
}

template<typename Key, typename Compare, typename Allocator>
void container_mem(const std::multiset<Key, Compare, Allocator>& obj) {
  using type = typename std::multiset<Key, Compare, Allocator>;
  using key_type = typename type::key_type;
  using key_compare = typename type::key_compare;
  using allocator_type = typename type::allocator_type;

  nameof_type<type>("std::multiset<Key, Compare, Allocator>");
  addressof_var(obj);
  sizeof_type<type>();
  sizeof_type<key_type>("Key");
  sizeof_type<key_compare>("Compare");
  sizeof_type<allocator_type>("Allocator");
  std::cout << "\tSize: " << obj.size() << '\n';
}

#if defined(DERPLIB_HAS_LIB_OPTIONAL)
template<typename T>
void container_mem(const std::optional<T>& obj) {
  using type = typename std::optional<T>;
  using value_type = typename type::value_type;

  nameof_type<type>("std::optional<T>");
  addressof_var(obj);
  sizeof_type<type>();
  sizeof_type<value_type>("T");
}
#endif  // defined(DERPLIB_HAS_LIB_OPTIONAL)

template<typename T, typename Container, typename Compare>
void container_mem(const std::priority_queue<T, Container, Compare>& obj) {
  using type = typename std::priority_queue<T>;
  using value_type = typename type::value_type;
#if __cplusplus > 201402L
  using compare_type = typename type::value_compare;
#endif  // __cplusplus > 201402L

  nameof_type<type>("std::priority_queue<T, Container, Compare>");
  addressof_var(obj);
  sizeof_type<type>();
  std::size_t vt_size = sizeof_type<value_type>("T");
#if __cplusplus > 201402L
  sizeof_type<compare_type>("Compare");
#endif  // __cplusplus > 201402L
  std::cout << "\tSize: " << obj.size() << '\n';
  std::cout << "\tMin Used Heap: " << vt_size * obj.size() << " B\n";
}

template<typename T, typename Container>
void container_mem(const std::queue<T, Container>& obj) {
  using type = typename std::queue<T, Container>;
  using value_type = typename type::value_type;

  nameof_type<type>("std::queue<T, Container>");
  addressof_var(obj);
  sizeof_type<type>();
  std::size_t vt_size = sizeof_type<value_type>("T");
  std::cout << "\tSize: " << obj.size() << '\n';
  std::cout << "\tMin Used Heap: " << vt_size * obj.size() << " B\n";
}

template<typename T, typename Container>
void container_mem(const std::stack<T, Container>& obj) {
  using type = typename std::stack<T, Container>;
  using value_type = typename type::value_type;

  nameof_type<type>("std::stack<T, Container>");
  addressof_var(obj);
  sizeof_type<type>();
  std::size_t vt_size = sizeof_type<value_type>("T");
  std::cout << "\tSize: " << obj.size() << '\n';
  std::cout << "\tMin Used Heap: " << vt_size * obj.size() << " B\n";
}

template<typename Key, typename Compare, typename Allocator>
void container_mem(const std::set<Key, Compare, Allocator>& obj) {
  using type = typename std::multiset<Key, Compare, Allocator>;
  using key_type = typename type::key_type;
  using key_compare = typename type::key_compare;
  using allocator_type = typename type::allocator_type;

  nameof_type<type>("std::set<Key, Compare, Allocator>");
  addressof_var(obj);
  sizeof_type<type>();
  std::size_t kt_size = sizeof_type<key_type>("Key");
  sizeof_type<key_compare>("Compare");
  sizeof_type<allocator_type>("Allocator");
  std::cout << "\tSize: " << obj.size() << '\n';
  std::cout << "\tMin Used Heap: " << kt_size * obj.size() << " B\n";
}

template<typename T>
void container_mem(const std::shared_ptr<T>& obj) {
  using type = typename std::shared_ptr<T>;
  using element_type = typename type::element_type;

  nameof_type<type>("std::shared_ptr<T>");
  addressof_var(obj);
  sizeof_type<type>();
  std::size_t et_size = sizeof_type<element_type>("T");
  std::cout << "\tMin Used Heap: " << (obj ? et_size : 0) << " B\n";
}

template<typename T, typename Deleter>
void container_mem(const std::unique_ptr<T, Deleter>& obj) {
  using type = typename std::unique_ptr<T, Deleter>;
  using element_type = typename type::element_type;
  using deleter_type = typename type::deleter_type;

  nameof_type<type>("std::unique_ptr<T, Deleter>");
  addressof_var(obj);
  sizeof_type<type>();
  std::size_t et_size = sizeof_type<element_type>("T");
  sizeof_type<deleter_type>("Deleter");
  std::cout << "\tMin Used Heap: " << (obj ? et_size : 0) << " B\n";
}

template<typename Key, typename T, typename Hash, typename KeyEqual, typename Allocator>
void container_mem(const std::unordered_map<Key, T, Hash, KeyEqual, Allocator>& obj) {
  using type = typename std::unordered_map<Key, T, Hash, KeyEqual, Allocator>;
  using key_type = typename type::key_type;
  using mapped_type = typename type::mapped_type;
  using value_type = typename type::value_type;
  using hasher = typename type::hasher;
  using key_equal = typename type::key_equal;
  using allocator_type = typename type::allocator_type;

  nameof_type<type>("std::unordered_map<Key, T, Hash, KeyEqual, Allocator>");
  addressof_var(obj);
  sizeof_type<type>();
  sizeof_type<key_type>("Key");
  sizeof_type<mapped_type>("T");
  sizeof_type<hasher>("Hash");
  sizeof_type<key_equal>("KeyEqual");
  sizeof_type<allocator_type>("Allocator");
  std::cout << "\tSize: " << obj.size() << '\n';
  std::cout << "\tMin Used Heap: " << sizeof(value_type) * obj.size() << " B\n";
}

template<typename Key, typename T, typename Hash, typename KeyEqual, typename Allocator>
void container_mem(const std::unordered_multimap<Key, T, Hash, KeyEqual, Allocator>& obj) {
  using type = typename std::unordered_multimap<Key, T, Hash, KeyEqual, Allocator>;
  using key_type = typename type::key_type;
  using mapped_type = typename type::mapped_type;
  using value_type = typename type::value_type;
  using hasher = typename type::hasher;
  using key_equal = typename type::key_equal;
  using allocator_type = typename type::allocator_type;

  nameof_type<type>("std::unordered_multimap<Key, T, Hash, KeyEqual, Allocator>");
  addressof_var(obj);
  sizeof_type<type>();
  sizeof_type<key_type>("Key");
  sizeof_type<mapped_type>("T");
  sizeof_type<hasher>("Hash");
  sizeof_type<key_equal>("KeyEqual");
  sizeof_type<allocator_type>("Allocator");
  std::cout << "\tSize: " << obj.size() << '\n';
  std::cout << "\tMin Used Heap: " << sizeof(value_type) * obj.size() << " B\n";
}

template<typename Key, typename Hash, typename KeyEqual, typename Allocator>
void container_mem(const std::unordered_set<Key, Hash, KeyEqual, Allocator>& obj) {
  using type = typename std::unordered_set<Key, Hash, KeyEqual, Allocator>;
  using key_type = typename type::key_type;
  using hasher = typename type::hasher;
  using key_equal = typename type::key_equal;
  using allocator_type = typename type::allocator_type;

  nameof_type<type>("std::unordered_set<Key, Hash, KeyEqual, Allocator>");
  addressof_var(obj);
  sizeof_type<type>();
  std::size_t kt_size = sizeof_type<key_type>("Key");
  sizeof_type<hasher>("Hash");
  sizeof_type<key_equal>("KeyEqual");
  sizeof_type<allocator_type>("Allocator");
  std::cout << "\tSize: " << obj.size() << '\n';
  std::cout << "\tMin Used Heap: " << kt_size * obj.size() << " B\n";
}

template<typename Key, typename Hash, typename KeyEqual, typename Allocator>
void container_mem(const std::unordered_multiset<Key, Hash, KeyEqual, Allocator>& obj) {
  using type = typename std::unordered_set<Key, Hash, KeyEqual, Allocator>;
  using key_type = typename type::key_type;
  using hasher = typename type::hasher;
  using key_equal = typename type::key_equal;
  using allocator_type = typename type::allocator_type;

  nameof_type<type>("std::unordered_multiset<Key, Hash, KeyEqual, Allocator>");
  addressof_var(obj);
  sizeof_type<type>();
  std::size_t kt_size = sizeof_type<key_type>("Key");
  sizeof_type<hasher>("Hash");
  sizeof_type<key_equal>("KeyEqual");
  sizeof_type<allocator_type>("Allocator");
  std::cout << "\tSize: " << obj.size() << '\n';
  std::cout << "\tMin Used Heap: " << kt_size * obj.size() << " B\n";
}

template<typename T, typename Allocator>
void container_mem(const std::vector<T, Allocator>& obj) {
  using type = typename std::vector<T, Allocator>;
  using value_type = typename type::value_type;
  using allocator_type = typename type::allocator_type;

  nameof_type<type>("std::vector<T, Allocator>");
  addressof_var(obj);
  sizeof_type<type>();
  std::size_t vt_size = sizeof_type<value_type>("T");
  sizeof_type<allocator_type>("Allocator");
  std::cout << "\tSize: " << obj.size() << '/' << obj.capacity() << '\n';
  std::cout << "\tMin Used Heap: " << vt_size * obj.size() << '/' << vt_size * obj.capacity() << " B\n";
}

}  // namespace util
}  // namespace derplib
