// Utilities for iterators.

#pragma once

#include <iterator>
#include <utility>

namespace derplib {
namespace util {

/**
 * Indexed specialization for @c std::for_each.
 *
 * The signature of the function should be equivalent to @code void f(IntegralType, const Type&); @endcode
 *
 * @tparam InputIt InputIterator
 * @tparam BinaryFunction Operand function type.
 * @param first Iterator to first element
 * @param last Iterator to last element
 * @param f Function to be applied to [first,last)
 * @return @c f
 */
// TODO: std::enable_if<...>
template<class InputIt, class BinaryFunction>
BinaryFunction for_each_indexed(InputIt first, InputIt last, BinaryFunction f) {
  std::size_t index = 0;
  for (; first != last; ++first, ++index) {
    f(index, *first);
  }
  return f;
}

#if __cplusplus > 199711L

using std::begin;
using std::end;
// TODO: Introduced in C++14
//using std::rbegin;
//using std::rend;

#else

template<class C>
typename C::iterator begin(C& c) {
  return c.begin();
}

template<class C>
typename C::const_iterator begin(const C& c) {
  return c.begin();
}

template<class T, std::size_t N>
T* begin(T (& array)[N]) {
  return &items[0];
}

template<class C>
typename C::iterator end(C& c) {
  return c.end();
}

template<class C>
typename C::const_iterator end(const C& c) {
  return c.end();
}

template<class T, std::size_t N>
T* end(T (& array)[N]) {
  return &items[N];
}

template<class C>
typename C::iterator rbegin(C& c) {
  return c.rbegin();
}

template<class C>
typename C::const_iterator rbegin(const C& c) {
  return c.rbegin();
}

template<class T, std::size_t N>
T* rbegin(T (& array)[N]) {
  return &items[N - 1];
}

template<class C>
typename C::iterator rend(C& c) {
  return c.rend();
}

template<class C>
typename C::const_iterator rend(const C& c) {
  return c.rend();
}

template<class T, std::size_t N>
T* rend(T (& array)[N]) {
  return &items[-1];
}

#endif  // __cplusplus > 199711L

}  // namespace util
}  // namespace derplib
