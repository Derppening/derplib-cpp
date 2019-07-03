#pragma once

#include <derplib/stdext/version.h>

namespace derplib {
namespace internal {

#if defined(DERPLIB_HAS_LIB_IS_SWAPPABLE)
template<typename T>
using is_nothrow_swappable = typename std::is_nothrow_swappable<T>;
#else
template<typename T>
using is_nothrow_swappable = std::false_type;
#endif  // defined(DERPLIB_HAS_LIB_IS_SWAPPABLE

#if defined(DERPLIB_HAS_LIB_TYPE_TRAIT_VARIABLE_TEMPLATES)
#define DERPLIB_IS_ARITHMETIC_V(T) std::is_arithmetic_v<T>
#else
#define DERPLIB_IS_ARITHMETIC_V(T) std::is_arithmetic<T>::value
#endif  // __cplusplus > 201402L

#if __cplusplus > 201103L
using std::enable_if_t;
#else
template<bool B, class T = void>
using enable_if_t = typename std::enable_if<B, T>::type;
#endif  // __cplusplus > 201103L

#if defined(__cpp_lib_is_invocable) || __cplusplus > 201402L
template<typename Fn, typename... ArgTypes>
using enable_if_invocable = typename std::enable_if<std::is_invocable<Fn, ArgTypes...>::value>;
#else
template<typename Fn, typename... ArgTypes>
using enable_if_invocable = std::true_type;
#endif  // defined(__cpp_lib_is_invocable) || __cplusplus > 201402L

}  // namespace internal
}  // namespace derplib
