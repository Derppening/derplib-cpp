// Utilities for iterators.

#pragma once

#include <iterator>
#include <utility>

namespace derplib {
inline namespace stdext {

/**
 * Indexed specialization for \c std::for_each.
 *
 * The signature of the function should be equivalent to \code void f(IntegralType, const Type&); \endcode
 *
 * \tparam InputIt InputIterator
 * \tparam BinaryFunction Operand function type.
 * \param first Iterator to first element
 * \param last Iterator to last element
 * \param f Function to be applied to [first,last)
 * \return \c f
 */
// TODO(Derppening): std::enable_if<...>
template<class InputIt, class BinaryFunction>
BinaryFunction for_each_indexed(InputIt first, InputIt last, BinaryFunction f) {
  for (std::size_t index{0}; first != last; ++first, ++index) {
    f(index, *first);
  }
  return f;
}

// TODO(Derppening): Introduced in C++14
// using std::rbegin;
// using std::rend;

}  // namespace stdext
}  // namespace derplib
